/*
 * gltraits.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLTRAITS_HPP_
#define GLTRAITS_HPP_

#ifndef MGL_REQUIREMENT
#error "You need to include a definition of OpenGL constants"
#endif

#include <cstdint>
#include <type_traits>
#include "glenum.hpp"
#include "globj.hpp"

namespace mgl {

namespace priv {

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_TESS_CONTROL_SHADER
#define GL_TESS_CONTROL_SHADER 0x8E88
#endif
#ifndef GL_TESS_EVALUATION_SHADER
#define GL_TESS_EVALUATION_SHADER 0x8E87
#endif
#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER 0x8DD9
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER 0x91B9 // TODO: should set the correct value here.
#endif

template<class T, bool = std::is_integral<T>::value >
struct priv_gl_buffer
{
    static constexpr GLenum target = GL_ARRAY_BUFFER;
    static constexpr GLenum usage  = GL_DYNAMIC_DRAW;
};

template<class T>
struct priv_gl_buffer<T, true>
{
    static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;
    static constexpr GLenum usage  = GL_STATIC_DRAW;
};

} /* namespace priv. */

/**
 * @brief Determines automatically the better target and usage for your type.
 *
 * You can use partial specialization of gl_buffer in order to get different
 * target and usage.
 *
 * Default behavior on types :
 *
 *  - Integers have the GL_ELEMENT_ARRAY_BUFFER target
 *  - Integers have the GL_STATIC_DRAW          usage
 *  - All others types T have the GL_ARRAY_BUFFER   target
 *  - All others types T have the GL_DYNAMIC_DRAW   usage.
 *
 * To create your own, you can specialize gl_buffer like so :
 *
 *      @code
 *          template<>
 *          struct gl_buffer_type<MyType>
 *          {
 *              // specify here the target and usage wanted
 *          };
 *      @endcode
 *
 * Or you can also create your own buffer type, but then you have
 * to explicitly specify it every time you want to use it. For instance,
 * with the gl_vector type, you would do like this :
 *
 *      @code
 *          gl_vector<T, MyBufferSpec>
 *      @endcode
 *
 */
template<class T>
struct gl_buffer_type : public priv::priv_gl_buffer<T>
{};

// -----------------------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------------------------------------------------//

/**
 * @brief gl_types offers typedef for opengl id and enum.
 */
struct gl_types
{
    typedef GLuint uid;
    typedef GLint  id;
    typedef GLenum en;
};

// -----------------------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------------------------------------------------//

/* namespace priv */
namespace priv {

template<typename T>
struct is_gl_attributes
{
    static constexpr bool value = false;
};

} /* namespace priv. */


} /* namespace mgl */


#endif /* GLTRAITS_HPP_ */
