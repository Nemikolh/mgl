/*
 * gltraits.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLTRAITS_HPP_
#define GLTRAITS_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include "../glrequires.hpp"
#include "glenum.hpp"
#include "glshader.hpp"

namespace mgl {

namespace priv {

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
 * \brief Determines automatically the better target and usage for your type.
 * You can use partial specialization of gl_buffer in order to get different
 * target and usage.
 *
 * Default behavior on types :
 *
 *  - Integers have the GL_ELEMENT_ARRAY_BUFFER target
 *  - Integers have the GL_STATIC_DRAW          usage
 *  - All others types T have GL_ARRAY_BUFFER   target
 *  - All others types T have GL_DYNAMIC_DRAW   usage.
 *
 * To create your own, you can specialize gl_buffer like so :
 *
 *      \code
 *          template<>
 *          struct gl_buffer_type<MyType>
 *          {
 *              // specify here the target and usage wanted
 *          };
 *      \endcode
 *
 * Or you can also create your own buffer type, but then you have
 * to explicitly specify it every time you want to use it. For instance,
 * with the gl_vector, you do like so :
 *
 *      \code
 *          gl_vector<T, MyBufferSpec>
 *      \endcode
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
    typedef GLuint id;
    typedef GLenum en;
};

// -----------------------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------------------------------------------------//

/*
 * Opengl object definitions.
 */
template<typename Buff>
struct gl_object_buffer
{
#ifdef NKH_TEST
    static int counter;
#endif

    static inline void gl_gen(GLsizei p_n, GLuint * p_buffers)
    {
#ifdef NKH_TEST
        counter += p_n;
#endif
        glCheck(glGenBuffers(p_n, p_buffers));
    }

    static inline void gl_bind(GLint p_id)
    {
        glCheck(glBindBuffer(Buff::target, p_id));
    }

    static inline void* gl_map_range(GLintptr p_offset, GLsizeiptr p_length, GLbitfield p_access)
    {
        return glMapBufferRange(Buff::target, p_offset, p_length, p_access);
    }

    static inline GLboolean gl_unmap()
    {
        return glUnmapBuffer(Buff::target);
    }

    static inline GLboolean gl_unmap(GLenum target)
    {
        return glUnmapBuffer(target);
    }

    static inline void gl_buffer_data(GLsizeiptr p_size, const GLvoid * p_data)
    {
        glCheck(glBufferData(Buff::target, p_size, p_data, Buff::usage));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteBuffers(p_n, p_buffers));
    }

    static inline void save_state()
    {
        // TODO
    }

    static inline void restore_state()
    {
        // TODO
    }
};

#ifdef NKH_TEST
template<typename T>
int gl_object_buffer<T>::counter = 0;
#endif

struct gl_object_framebuffer
{
    static inline void gl_gen(GLsizei p_n, GLuint * p_buffers)
    {
        glCheck(glGenFramebuffers(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteFramebuffers(p_n, p_buffers));
    }
};

struct gl_object_texture
{
    static inline void gl_gen(GLsizei p_n, GLuint * p_buffers)
    {
        glCheck(glGenTextures(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteTextures(p_n, p_buffers));
    }
};

struct gl_object_transformfeedback
{
    static inline void gl_gen(GLsizei p_n, GLuint * p_buffers)
    {
        glCheck(glGenTransformFeedbacks(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteTransformFeedbacks(p_n, p_buffers));
    }
};

struct gl_object_vertexarrays
{
    static inline void gl_gen(GLsizei p_n, GLuint * p_vao)
    {
        glCheck(glGenVertexArrays(p_n, p_vao));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_vao)
    {
        glCheck(glDeleteVertexArrays(p_n, p_vao));
    }

    static inline void gl_bind(GLuint p_vao)
    {
        glCheck(glBindVertexArray(p_vao));
    }
};


// -----------------------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------------------------------------------------//

/**
 * \class has_typedef_gl_object_type test for the presence or absence of the nested typedef
 * gl_object_type.
 * This is a conformant integral metafunction that can be used with the boost::mpl library.
 */
template<typename T>
class has_typedef_gl_object_type
{
    template<typename U>
    static std::true_type  deduce(U*, typename U::gl_object_type* = 0);
    static std::false_type deduce(...);

public:

    typedef decltype(deduce(static_cast<T*>(0))) type;
    static constexpr bool value = type::value;
};

/**
 * \class has_save_restore_state test for the presence of the static functions save_state() and restore_state().
 * This is a conformant integral metafunction that can be used with the boost::mpl library.
 */
template<typename T>
class has_save_restore_state
{
    template<typename F, F f> class Helper{};

    template<typename U>
    static std::true_type  deduce(U*, Helper<void (*)(), &U::save_state>* = 0, Helper<void (*)(), &U::restore_state>* = 0);
    static std::false_type deduce(...);

public:

    typedef decltype(deduce(static_cast<T*>(0))) type;
    static constexpr bool value = type::value;
};

/* namespace priv. */
namespace priv {

template<typename GLState, bool = has_save_restore_state<GLState>::value >
struct gl_state_traits
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    /**
     * \brief Save the current state if a gl_object_type typedef
     * is provided.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<has_typedef_gl_object_type<U>::value, void>::type
    save_state()
    {
        U::gl_object_type::save_state();
    }

    /**
     * \brief Does nothing.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<!has_typedef_gl_object_type<U>::value, void>::type
    save_state()
    {}

    /**
     * \brief Restore the current state if a gl_object_type typedef
     * is provided.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<has_typedef_gl_object_type<U>::value, void>::type
    restore_state()
    {
        U::gl_object_type::restore_state();
    }

    /**
     * \brief Does nothing.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<!has_typedef_gl_object_type<U>::value, void>::type
    restore_state()
    {}
};

template<typename GLState>
struct gl_state_traits<GLState, true>
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    /**
     * \brief Save the state using the static functions provided.
     */
    static inline constexpr void save_state()
    {
        GLState::save_state();
    }

    /**
     * \brief Restore the state using the static functions provided.
     */
    static inline constexpr void restore_state()
    {
        GLState::restore_state();
    }

};

} /* namespace priv. */

/**
 * \class gl_state_traits is an helper for storing gl_state.
 * This class is mainly used for the gl_scope class.
 * If you want to preserve an OpenGL state you have the following options :
 *
 *      1.  You can provide a partial template specialization of gl_state_traits (it'll save a bit of compile time template instantiation )
 *
 *      2. You can provide nested static functions that performs all the job, named
 *        \code save_state(); \endcode and \code restore_state(); \endcode
 *
 *      3. You can provide the nested typedef ::gl_object_type which can be one of the following
 *          * gl_object_buffer
 *          * gl_object_framebuffer
 *          * gl_object_program
 *          * gl_object_shader
 *          * gl_object_texture
 *          * gl_object_transformfeedback
 *          * gl_object_vertexarrays
 *
 *      4. And finally you can do nothing at all if you don't want any state preservation.
 *
 * You can also provide all those options and they'll be picked up in this order.
 */
template<typename GLState>
struct gl_state_traits : public priv::gl_state_traits<GLState>
{};


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
