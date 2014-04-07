#ifndef SHADERPROPERUSE_H_
#define SHADERPROPERUSE_H_

#include <cxxtest/TestSuite.h>
#include <SFML/Graphics.hpp>

#include "../mgl/glrequires.hpp"
#include "../mgl/glvector.hpp"
#include "../mgl/type/glshader.hpp"
#include "../mgl/glexceptions.hpp"

using namespace mgl;

class ShaderProperUse : public CxxTest::TestSuite
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

    void testCompileShader()
    {
        gl_shader shader(shader_type::VERTEX_SHADER);
        TS_ASSERT_THROWS(shader.load_src("abc"), gl_compile_error&);
        TS_ASSERT_THROWS_NOTHING(priv::glTryError());

        TS_ASSERT_THROWS_NOTHING(shader.load_src("#version 330\nin vec4 vertex;\nvoid main(void){gl_Position = vertex;}"));
        TS_ASSERT_THROWS_NOTHING(priv::glTryError());
    }

};

#endif /*SHADERPROPERUSE_H_*/
