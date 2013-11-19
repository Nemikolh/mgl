#ifndef GLVECTORPROPERUSE_H_
#define GLVECTORPROPERUSE_H_

#include <cxxtest/TestSuite.h>

#include "../src/core/gl/glvector.hpp"
#include "../src/core/gl/glrequires.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace nkh::core::gl;

class GLVectorProperUse : public CxxTest::TestSuite
{
    std::unique_ptr<sf::Window> window;
public:

    void setUp()
	{
        sf::ContextSettings settings;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        std::cout << "Test Suite Launched" << std::endl;
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
#ifdef NKH_TEST
	    TS_ASSERT_EQUALS(gl_object_buffer<gl_buffer<float>>::counter, 3);
#endif
	}

};

#endif /*GLVECTORPROPERUSE_H_*/
