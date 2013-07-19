/*
 * gltraits.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLTRAITS_HPP_
#define GLTRAITS_HPP_

#include <cstdint>
#include "../glrequires.hpp"
#include <memory>

namespace nkh {
namespace core {
namespace gl {

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
 *          struct gl_buffer<MyType>
 *          {
 *              // specify here the target and usage wanted
 *          };
 *      \endcode
 *
 * Or you can also create your own buffer type, but then you have
 * to explicitly specify it everytime you want to use it. For instance,
 * with the gl_vector, you do like so :
 *
 *      \code
 *          gl_vector<T, MyBufferSpec>
 *      \endcode
 *
 */
template<class T>
struct gl_buffer : public priv::priv_gl_buffer<T>
{};

/**
 * \class tuple_component_type is a MetaFunction to specify the type of your homogen tuple type.
 *
 */
template<typename T>
struct tuple_component_type
{
    static constexpr GLenum value = GL_FLOAT;
};

/*
 * Opengl object definitions.
 */

struct gl_object_buffer
{
    static inline void gl_gen(GLsizei p_n​, GLuint * p_buffers)
    {
        glCheck(glGenBuffers(p_n, p_buffers));
    }

    static inline void gl_bind(GLint p_id)
    {
        glBindBuffer(p_id);
    }

    static inline void* gl_map_range(GLenum p_target​, GLintptr p_offset​, GLsizeiptr p_length​, GLbitfield p_access​)
    {
        return glMapBufferRange(p_target, p_offset, p_length, p_access);
    }

    static inline GLboolean gl_unmap(GLenum target​)
    {
        return glUnmapBuffer(p_target);
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteBuffers(p_n​, p_buffers​));
    }
};

struct gl_object_framebuffer
{
    static inline void gl_gen(GLsizei p_n​, GLuint * p_buffers)
    {
        glCheck(glGenFramebuffers(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteFramebuffers(p_n​, p_buffers​));
    }
};


struct gl_object_program
{

    static inline GLuint gl_gen()
    {
        return glCreateProgram();
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        for(int i = 0; i < p_n; ++i)
            glDeleteProgram(p_buffers​[i]);
        glCheck();
    }
};

struct gl_object_shader
{
    /**
     * \brief Generate an id for ShaderType.
     * \return Returns the generated id.
     */
    template<GLenum ShaderType>
    static inline GLuint gl_gen()
    {
        return glCreateShader(ShaderType);
    }

//    /*
//     * Here we use specialization, to give the user
//     * a warning on the availability of a shader or not.
//     * Actually, that's seems pretty useless.
//     */
//#ifdef GL_VERTEX_SHADER
//    template<>
//    static GLuint gl_gen<GL_VERTEX_SHADER>()
//    {
//        return glCreateShader(GL_VERTEX_SHADER);
//        //glCheckCall(glGenBuffers(p_n, p_buffers));
//    }
//#endif

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        for(int i = 0; i < p_n; ++i)
            glDeleteShader(p_buffers​[i]);
        glCheck();
    }
};


struct gl_object_texture
{
    static inline void gl_gen(GLsizei p_n​, GLuint * p_buffers)
    {
        glCheck(glGenTextures(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteTextures(p_n​, p_buffers​));
    }
};

struct gl_object_transformfeedback
{
    static inline void gl_gen(GLsizei p_n​, GLuint * p_buffers)
    {
        glCheck(glGenTransformFeedbacks(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteTransformFeedbacks(p_n​, p_buffers​));
    }
};

struct gl_object_vertexarrays
{
    static inline void gl_gen(GLsizei p_n​, GLuint * p_buffers)
    {
        glCheck(glGenVertexArrays(p_n, p_buffers));
    }

    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        glCheck(glDeleteVertexArrays(p_n​, p_buffers​));
    }
};


namespace priv {

template<typename T>
struct is_gl_attributes
{
    static constexpr bool value = false;
};

} /* namespace priv. */


} /* namespace gl */
} /* namespace core */
} /* namespace nkh */


#endif /* GLTRAITS_HPP_ */
