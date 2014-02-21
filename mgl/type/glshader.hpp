/*
 * glshader.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GL_TYPE_SHADER_HPP_
#define GL_TYPE_SHADER_HPP_

#include <boost/noncopyable.hpp>
#include "gltraits.hpp"
#include "../glexceptions.hpp"

namespace mgl {

enum class shader_type
{
    VERTEX_SHADER,
    TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER,
    COMPUTE_SHADER
};

struct gl_shader
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_shader() noexcept
        : m_shader_id(0)
        , m_shader_type(shader_type::VERTEX_SHADER)
    {}

    gl_shader(shader_type p_type) noexcept
        : m_shader_id(0)
        , m_shader_type(p_type)
    {}

    /**
     * @brief Copy isn't allowed.
     */
    gl_shader(const gl_shader&) = delete;
    const gl_shader& operator=(const gl_shader&) = delete;

    /**
     * @brief Move constructor.
     * @param p_rhs is the temporary moved here.
     */
    gl_shader(gl_shader&& p_rhs) noexcept
        : m_shader_id(p_rhs.m_shader_id)
        , m_shader_type(p_rhs.m_shader_type)
    {
        p_rhs.m_shader_id = 0;
    }

    const gl_shader& operator=(gl_shader&& p_rhs)
    {
        release();
        m_shader_id = p_rhs.m_shader_id;
        m_shader_type = p_rhs.m_shader_type;
        p_rhs.m_shader_id = 0;
        return *this;
    }

    ~gl_shader()
    {
        release();
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Load the passed source to this shader.
     * @param p_src is the source to load.
     */
    void load_src(const char* p_src)
    {
        static_assert(sizeof(char) == sizeof(GLchar), "This function will not work properly.");
        // ------------------------- DECLARE ------------------------ //

        ensure_created();
        gl_object_shader::gl_shader_source(m_shader_id, 1, &p_src, nullptr);
        gl_object_shader::gl_compile(m_shader_id);

        if(!gl_object_shader::gl_compile_status(m_shader_id))
            throw gl_compile_error(gl_object_shader::gl_info_log(m_shader_id));
    }

    /**
     * @brief Load the passed source to this shader.
     * @param p_src is the string source to load.
     */
    template<typename String>
    void load_src(String p_src)
    {
        auto c_str = p_src.c_str();
        // ------------------------- DECLARE ------------------------ //

        load_src(c_str);
    }

    gl_types::id id() const
    {
        return m_shader_id;
    }

private:
    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    void ensure_created()
    {
        if(!m_shader_id)
            m_shader_id = gl_object_shader::gl_gen(convert_to_en(m_shader_type));
    }

    void release()
    {
        if(m_shader_id)
            gl_object_shader::gl_delete(1, &m_shader_id);
    }

    GLenum convert_to_en(shader_type p_tag) const
    {
        switch (p_tag) {
            case shader_type::VERTEX_SHADER:
                return GL_VERTEX_SHADER;
            case shader_type::TESS_CONTROL_SHADER:
                return GL_TESS_CONTROL_SHADER;
            case shader_type::TESS_EVALUATION_SHADER:
                return GL_TESS_EVALUATION_SHADER;
            case shader_type::GEOMETRY_SHADER:
                return GL_GEOMETRY_SHADER;
            case shader_type::FRAGMENT_SHADER:
                return GL_FRAGMENT_SHADER;
            case shader_type::COMPUTE_SHADER:
                return GL_COMPUTE_SHADER;
        }
    }


    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The id of the shader. */
    gl_types::id m_shader_id;
    /** The type of the shader. */
    shader_type m_shader_type;
};

}  /* namespace mgl */


#endif /* GL_TYPE_SHADER_HPP_ */
