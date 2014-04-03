#ifndef GLVECTORPROPERUSE_H_
#define GLVECTORPROPERUSE_H_

#include <cxxtest/TestSuite.h>

#include "../mgl/glrequires.hpp"
#include "../mgl/glvector.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

using namespace mgl;

class GLVectorProperUse : public CxxTest::TestSuite
{
    std::unique_ptr<sf::Window> window;
public:

    void setUp()
	{
        sf::ContextSettings settings;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        window.reset(new sf::Window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings));
        window->setVisible(false);

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::cerr << "GLEW error: " << glewGetErrorString(err) << std::endl;
        }

        if (!glewIsSupported("GL_VERSION_3_0"))
        {
            std::cerr << "OpenGL version 3.0 isn't supported." << std::endl;
        }
	}

	void tearDown()
	{
	    window->close();
	    window.reset();
        gl_object_buffer<gl_buffer_type<float>>::counter = 0;
	}

	void testCreation()
	{
	    TS_TRACE("Constructor with std::initializer_list");
	    gl_vector<float> test = { 1.0f, 2.0f, 3.0f, 4.0f };
	    TS_ASSERT_THROWS_NOTHING(priv::glTryError());

	    TS_TRACE("Copy constructor");
	    gl_vector<float> copy(test);
	    TS_ASSERT_THROWS_NOTHING(priv::glTryError());
	    TS_ASSERT_EQUALS(copy == test, true);

	    TS_ASSERT_EQUALS(test.is_mapped(), false);
	    TS_ASSERT_EQUALS(copy.is_mapped(), false);

#       ifdef NKH_TEST
            TS_TRACE("Additional Test : Number of buffers");
            TS_ASSERT_EQUALS(gl_object_buffer<gl_buffer_type<float>>::counter, 2);
#       endif
	}

	void testBinders()
	{
	    TS_TRACE("Test the behavior when not mapped");
	    float test_host[4]    = { 5.0f, 4.0f, 8.0f, 2.0f };
	    gl_vector<float> test = { 5.0f, 4.0f, 8.0f, 2.0f };
	    TS_ASSERT_THROWS_NOTHING(priv::glTryError());

	    gl_vector<float> other = { 1.0f, 2.0f, 3.0f, 4.0f };
	    float other_host[4]    = { 1.0f, 2.0f, 3.0f, 4.0f };

	    // Scope for mapping state.
	    {
            TS_TRACE("Bind at scope");
            auto lock = bind_at_scope(test);
            for(int i = 0; i < test.size(); ++i)
            {
                TS_ASSERT_EQUALS(test_host[i], test.at(i));
            }

            TS_TRACE("Bind and apply a lambda");
            bind_and_apply(other, [&](){
                int i{0};
                for(auto el : other)
                {
                    TS_ASSERT_EQUALS(other_host[i++], el);
                }
            });

            TS_ASSERT_THROWS_NOTHING(priv::glTryError());
	    }

	    TS_ASSERT_EQUALS(test.is_mapped(), false);
	    TS_ASSERT_EQUALS(other.is_mapped(), false);

#       ifdef NKH_TEST
            TS_TRACE("Additional Test : Number of buffers");
            TS_ASSERT_EQUALS(gl_object_buffer<gl_buffer_type<float>>::counter, 2);
#       endif
	}

	void testForceReallocation()
	{
        TS_TRACE("Forcing reallocation with push_back.");
        gl_vector<float> test;
        std::vector<float> valid;

        // Scope for mapping state.
        {
            auto lock = bind_at_scope(test);

            for(int i = 0; i < 20; ++i)
            {
                test.push_back(i);
                valid.push_back(i);
            }

            TS_ASSERT_THROWS_NOTHING(priv::glTryError());

            for(int i = 0; i < 20; i++)
            {
                TS_ASSERT_EQUALS(test[i], valid[i]);
            }
        }

        TS_ASSERT_EQUALS(test.is_mapped(), false);

#       ifdef NKH_TEST
            TS_TRACE("Additional Test : Number of buffers");
            TS_ASSERT_EQUALS(gl_object_buffer<gl_buffer_type<float>>::counter, 6);
#       endif
	}

};

#endif /*GLVECTORPROPERUSE_H_*/
