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

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

// TODO: Remove this include as it won't be part of the library.
#include "../mgl/glrequires.hpp"
/* This header provides a wrapper around the calls to glDraw* */
#include "../mgl/gldraw.hpp"
/* With gldata.hpp, we can defines our attributes in a simple standard layout. */
#include "../mgl/gldata.hpp"



MGL_DEFINE_GL_ATTRIBUTES(
        (),         // The namespace sequence. For instance, (a)(b)(c) is equivalent  to a::b::c
        vertex,     // The struct name.
        (glm::vec3, position)
        (glm::vec3, color))

int main(int argc, char **argv)
{

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
}


