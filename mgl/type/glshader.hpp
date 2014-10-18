/*
 * glshader.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GL_TYPE_SHADER_HPP_
#define GL_TYPE_SHADER_HPP_

#include <cassert>
#include "gltraits.hpp"
#include "../glexceptions.hpp"
#include "priv/details.hpp"

/**
 * @defgroup shader Shaders module
 * @brief Classes linked to the creation, manipulation of shaders and compile-time shaders.
 *
 * gl_shader and gl_program have reference-counting semantics. \n
 * This choice of design is justified on the fact that most of the time you don't
 * want to create a deep copy of your shader. That is if you use copy-construction,
 * you don't really intend to get a new shader id, and eventually wait for a new compilation step to occurs. \n
 * What you do want is say: "Give this shader to this game entity in its rendering options."\n
 * Using reference counting is then a way to ease the use of gl_shader for the developer. \n
 * We could have used move semantic to prevent this behavior of copying, \n
 * but it seems reasonable to assume that you'll need shared semantics here.
 *
 */
namespace mgl {

/**
 * @ingroup shader
 * @brief mgl enums for the different shader stages.
 */
enum class shader_type
{
    VERTEX_SHADER,         //!< VERTEX_SHADER
    TESS_CONTROL_SHADER,   //!< TESS_CONTROL_SHADER
    TESS_EVALUATION_SHADER,//!< TESS_EVALUATION_SHADER
    GEOMETRY_SHADER,       //!< GEOMETRY_SHADER
    FRAGMENT_SHADER,       //!< FRAGMENT_SHADER
    COMPUTE_SHADER,        //!< COMPUTE_SHADER
    Count                  //!< Count for internal use only.
};

/**
 * @ingroup shader
 * @brief Simple wrapper around OpenGL Shaders.
 *
 * This class use reference counting semantic when constructing
 * a new gl_shader object by copy-constructor.\n
 * @see gl_program
 */
struct gl_shader : public priv::base_id_ref_count
{
    typedef priv::base_id_ref_count base;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_shader() noexcept
        : base()
        , m_shader_type(shader_type::VERTEX_SHADER)
    {}

    explicit gl_shader(shader_type p_type) noexcept
        : base()
        , m_shader_type(p_type)
    {}

    /**
     * @brief Copy constructor.
     */
    gl_shader(const gl_shader& p_rhs) = default;
    gl_shader& operator=(const gl_shader& p_rhs) = default;

    /**
     * @brief Move constructor.
     * @param p_rhs is the temporary moved here.
     */
    gl_shader(gl_shader&& p_rhs) = default;
    gl_shader& operator=(gl_shader&& p_rhs) = default;

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
        gl_types::id id = this->id();
        gl_object_shader::gl_shader_source(id, 1, &p_src, nullptr);
        gl_object_shader::gl_compile(id);

        if(!gl_object_shader::gl_compile_status(id))
            throw gl_compile_error(gl_object_shader::gl_info_log(id));
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

    /**
     * @brief Returns the shader's type.
     * @return Returns the shader's type.
     */
    shader_type type() const
    {
        return m_shader_type;
    }

private:
    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    GLenum convert_to_en(shader_type p_tag) const
    {
        switch (p_tag) {
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
            case shader_type::VERTEX_SHADER:
            default:
                return GL_VERTEX_SHADER;
        }
    }

    gl_types::id gen() override
    {
        return gl_object_shader::gl_gen(convert_to_en(m_shader_type));
    }

    void gl_delete(gl_types::id p_id) override
    {
        gl_object_shader::gl_delete(1, &p_id);
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The type of the shader. */
    shader_type m_shader_type;
};

}  /* namespace mgl */

#endif /* GL_TYPE_SHADER_HPP_ */
