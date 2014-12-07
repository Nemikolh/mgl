/*
 * gltexture.hpp
 * author: nemikolh
 */

#pragma once
#include <_setup.hpp>

#if MGL_GL_VERSION > 32
#include <_core/type/gltexture.hpp>
#elif defined(MGL_GL_ES)
#error "mgl_es version does not define this type. "
#else
#error "GL headers not included, also you have to compile against GL_VERSION_3_3 at least"
#endif
