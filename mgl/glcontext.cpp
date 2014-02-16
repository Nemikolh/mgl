/*
 * glcontext.cpp
 *
 *  Created on: 28 juin 2013
 *      Author: nemikolh
 */

#include "glcontext.h"
#include "glformat.hpp"

#include <stack>
#include <unordered_map>
#include <cstdint>
#include <iostream>

namespace nkh {
namespace core {
namespace gl {

gl_context::gl_context(const gl_format & p_format):
    m_impl(new priv::gl_context_impl(p_format))
{
}

gl_context::~gl_context()
{
    delete m_impl;
}

namespace priv {
class gl_context_impl
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_context_impl(gl_format p_format):
        m_format(p_format)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    void bind(GLenum p_target, GLuint p_buffer)
    {
        glCheck(glBindBuffer(p_target, p_buffer));
        //m_stack.push(std::make_pair(p_target, p_buffer));
        auto stack = m_mapStacks[p_target];
        stack.push(p_buffer);
    }

    void unbind(GLenum p_target)
    {
        auto stack = m_mapStacks[p_target];
        stack.pop();

        if(!stack.empty())
        {
            glCheck(glBindBuffer(p_target, stack.top()));
        }
        else
        {
            glCheck(glBindBuffer(p_target, 0));
        }
    }

    template<typename ObjType>
    void decrement_count(GLuint p_id);

    template<typename ObjType>
    void increment_count(GLuint p_id);

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The gl_format associated to this context. */
    gl_format           m_format;

    /** The stack for bounded buffers. */
    std::unordered_map<GLenum, std::stack<GLuint> >  m_mapStacks;

    /** The reference counter number for buffers ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountBuffers;
    /** The reference counter number for Textures ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountTextures;
    /** The reference counter number for Shaders ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountShaders;
    /** The reference counter number for Programs ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountPrograms;
    /** The reference counter number for FrameBuffers ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountFrameBuffers;
    /** The reference counter number for VAO ids. */
    std::unordered_map<GLuint, std::uint16_t> m_mapIdsCountVAO;
};

template<>
void gl_context_impl::decrement_count<gl_object_buffer>(GLuint p_id)
{
    auto item = m_mapIdsCountBuffers.find(p_id);
    if(item != m_mapIdsCountBuffers.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_buffer::gl_delete(1, &p_id);
            m_mapIdsCountBuffers.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

template<>
void gl_context_impl::decrement_count<gl_object_texture>(GLuint p_id)
{
    auto item = m_mapIdsCountTextures.find(p_id);
    if(item != m_mapIdsCountTextures.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_texture::gl_delete(1, &p_id);
            m_mapIdsCountTextures.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

template<>
void gl_context_impl::decrement_count<gl_object_shader>(GLuint p_id)
{
    auto item = m_mapIdsCountShaders.find(p_id);
    if(item != m_mapIdsCountShaders.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_shader::gl_delete(1, &p_id);
            m_mapIdsCountShaders.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

template<>
void gl_context_impl::decrement_count<gl_object_program>(GLuint p_id)
{
    auto item = m_mapIdsCountPrograms.find(p_id);
    if(item != m_mapIdsCountPrograms.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_program::gl_delete(1, &p_id);
            m_mapIdsCountPrograms.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

template<>
void gl_context_impl::decrement_count<gl_object_framebuffer>(GLuint p_id)
{
    auto item = m_mapIdsCountFrameBuffers.find(p_id);
    if(item != m_mapIdsCountFrameBuffers.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_framebuffer::gl_delete(1, &p_id);
            m_mapIdsCountFrameBuffers.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

template<>
void gl_context_impl::decrement_count<gl_object_vertexarrays>(GLuint p_id)
{
    auto item = m_mapIdsCountVAO.find(p_id);
    if(item != m_mapIdsCountVAO.end())
    {
        item->second -= 1;

        if(item->second == 0)
        {
            gl_object_vertexarrays::gl_delete(1, &p_id);
            m_mapIdsCountVAO.erase(item);
        }
    }
#ifndef MGL_NDEBUG
    else
    {
        std::cerr << "Error, count decremented for deleted buffer : " << p_id << std::endl;
    }
#endif
}

}

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
