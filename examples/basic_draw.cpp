/*
 * basic_draw.cpp
 *
 *  Created on: 4 apr. 2014
 *      Author: nemikolh
 *
 *  This example shows the basic use of the library
 *  to render with OpenGL. It shows the different part
 *  of the library that can be used to render simply
 *  some geometry data.
 *  This example use the SFML, but this is not a requirement at all.
 *  In the example we also make use of GLM. Event if this is also here
 *  not a requirement, in order to get it work the library provides
 *  implementation required for it. Thus it is
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

// TODO: Remove this include as it won't be part of the library.
#include "../mgl/glrequires.hpp"
/* This header provides a wrapper around the calls to glDraw* */
#include "../mgl/gldraw.hpp"
/* With gldata.hpp, we can defines our attributes in a simple standard layout. */
#include "../mgl/gldata.hpp"
/* We use the extension library here, providing facilities to generate simple shaders. */
#include "../mgl/extension/default_shaders.hpp"


MGL_DEFINE_GL_ATTRIBUTES(
        (),         // The namespace sequence. For instance, (a)(b)(c) is equivalent  to a::b::c
        vertex,     // The struct name.
        (glm::vec3, position)
        (glm::vec3, color))

int main(int argc, char **argv)
{
    std::cout << "Example 1" << std::endl;

    // ------------------------- Window Creation ------------------------ //
    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    std::unique_ptr<sf::Window> window(new sf::Window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings));

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "GLEW error: " << glewGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    if (!glewIsSupported("GL_VERSION_3_3"))
    {
        std::cerr << "OpenGL version 3.0 isn't supported." << std::endl;
        return EXIT_FAILURE;
    }

    // ------------------------- DATA DEFINITION ------------------------ //

    mgl::gl_vector<vertex> data = {
            { glm::vec3{ 0.0, 0.0 ,0.0 }, glm::vec3{ 0., 1., 0. } },
            { glm::vec3{ 0.0, 0.5 ,0.0 }, glm::vec3{ 0., 0., 1. } },
            { glm::vec3{ 0.5, 0.0 ,0.0 }, glm::vec3{ 1., 0., 0. } }
    };

    mgl::gl_vector<std::uint32_t> indices = {
            0, 1, 2
    };

    // ------------------------ RENDERING CREATION ----------------------- //

    mgl::gl_program prog;
    mgl::gl_shader vert_sh;
    mgl::gl_shader frag_sh;

    try
    {
        vert_sh = mgl::extension::pass_through_shader<vertex>(mgl::shader_type::VERTEX_SHADER);
        frag_sh = mgl::extension::pass_through_shader<vertex>(mgl::shader_type::FRAGMENT_SHADER);
    }
    catch(const mgl::gl_exception & e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    prog.attach(vert_sh);
    prog.attach(frag_sh);

    try
    {
        prog.link();
    }
    catch(const mgl::gl_exception & e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    mgl::gl_vao vao;
    vao = prog.make_vao(data, indices);

    // -------------------------- RENDERING LOOP ------------------------- //

    std::chrono::milliseconds duration(10);
    while(window->isOpen())
    {
        // Process events
       sf::Event event;
       while (window->pollEvent(event))
       {
           // Close window : exit
           if (event.type == sf::Event::Closed)
           {
               window->close();
               return EXIT_SUCCESS;
           }
       }

       glClearColor(0.f, 0.f, 0.f, 0.f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       prog.use();
       mgl::gl_draw(vao);

       window->display();

       std::this_thread::sleep_for(duration);
    }
}


