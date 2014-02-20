/*
 * globj.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLOBJ_HPP_
#define GLOBJ_HPP_

#include "globjsh.hpp"

namespace mgl {

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


}  /* namespace mgl */

#endif /* GLOBJ_HPP_ */
