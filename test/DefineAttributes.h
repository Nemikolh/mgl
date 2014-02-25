#ifndef DEFINEATTRIBUTES_H_
#define DEFINEATTRIBUTES_H_

#include <iostream>
#include <cxxtest/TestSuite.h>
#include "../mgl/glrequires.hpp"
#include <SFML/Graphics.hpp>
#include "../mgl/gldata.hpp"

MGL_DEFINE_GL_ATTRIBUTES((tmp), test, (char, pos)(double, length))

class DefineAttributes : public CxxTest::TestSuite
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
    }

    void testDefinition()
    {
        std::size_t offset = mgl::offset_at<tmp::test, 0>::value;
        TS_ASSERT_EQUALS(offset, 0);

        offset = mgl::offset_at<tmp::test, 1>::value;
        TS_ASSERT_EQUALS(offset, sizeof(double));
    }
};

#endif /*DEFINEATTRIBUTES_H_*/
