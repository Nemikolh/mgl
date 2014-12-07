/*
 * _setup.hpp
 * author: nemikolh
 */

#pragma once

/**
 * This header set the configuration for using mgl.
 * You have the following options:
 *  - include the gl headers yourself.
 *  - define MGL_USE_GLEW that will include for you the glew.h header.
 *  - define MGL_USE_ES2 that will include for you GLES2 header.
 */
#ifdef MGL_USE_GLEW
#include <GL/glew.h>
#elif defined(MGL_USE_ES2)
#include <GLES2/gl2ext.h>
#include <GLES2/gl2.h>
#define MGL_GL_ES 20
#endif

#define MGL_GL_VERSION 10

#ifdef GL_VERSION_3_3
#define MGL_GL_VERSION 33
#endif

#ifdef GL_VERSION_4_0
#define MGL_GL_VERSION 40
#endif

#ifdef GL_VERSION_4_1
#define MGL_GL_VERSION 41
#endif

#ifdef GL_VERSION_4_2
#define MGL_GL_VERSION 42
#endif

#ifdef GL_ES_VERSION_2_0
#define MGL_GL_ES 20
#endif
