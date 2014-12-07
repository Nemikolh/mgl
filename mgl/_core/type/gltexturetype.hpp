/*
 * gltexturetype.hpp
 *
 *  Created on: 15 nov. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLTEXTURETYPE_HPP_
#define MGL_TYPE_GLTEXTURETYPE_HPP_

#include "../glrequires.hpp"

namespace mgl {

namespace priv {

struct texture_type_1D {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 1;

    static constexpr GLenum target = GL_TEXTURE_1D;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_1D_array {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = true;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 1;

    static constexpr GLenum target = GL_TEXTURE_1D_ARRAY;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_buffer {

    static constexpr bool hasMipmaps      = false;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 1;

    static constexpr GLenum target = GL_TEXTURE_BUFFER;

    // No alloc possible for buffer target.
};

struct texture_type_2D {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 2;

    static constexpr GLenum target = GL_TEXTURE_2D;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_2D_array {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = true;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 2;

    static constexpr GLenum target = GL_TEXTURE_2D_ARRAY;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_cube_map {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = true;

    static constexpr unsigned int dimension = 2;

    static constexpr GLenum target = GL_TEXTURE_CUBE_MAP;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_cube_map_array {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = true;
    static constexpr bool hasCubemapFaces = true;

    static constexpr unsigned int dimension = 2;

    static constexpr GLenum target = GL_TEXTURE_CUBE_MAP_ARRAY;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_rectangle {

    static constexpr bool hasMipmaps      = false;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 2;

    static constexpr GLenum target = GL_TEXTURE_RECTANGLE;

    static void alloc_immutable();
    static void alloc_mutable();
};

struct texture_type_3D {

    static constexpr bool hasMipmaps      = true;
    static constexpr bool hasArrayLayers  = false;
    static constexpr bool hasCubemapFaces = false;

    static constexpr unsigned int dimension = 3;

    static constexpr GLenum target = GL_TEXTURE_3D;

    static void alloc_immutable();
    static void alloc_mutable();
};



}  /* namespace priv */

}  /* namespace mgl */

#include "gltexturetype.inl"

#endif /* MGL_TYPE_GLTEXTURETYPE_HPP_ */
