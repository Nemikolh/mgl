/*
 * glprogram.hpp
 *
 *  Created on: 21 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLPROGRAM_HPP_
#define GLPROGRAM_HPP_

#include <utility>
#include <type_traits>
#include "glvao.hpp"
#include "glshader.hpp"
#include "../glexceptions.hpp"
#include "../meta/glmhelper.hpp"
#include "../meta/gluniformhelpers.hpp"
#include "gluniform.hpp"
#include "priv/details.hpp"

namespace mgl {

/**
 * @ingroup shader
 * @brief gl_program is a wrapper for an opengl shader program.
 *
 * With your gl_program object, you can directly set uniform value.
 *
 * @see gl_shader
 */
struct gl_program : public priv::base_id_ref_count
{
    typedef priv::base_id_ref_count base;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Default constructor.
     */
    gl_program() noexcept
        : base()
    {}

    /**
     * @brief Default copy constructor.
     */
    gl_program(const gl_program&) = default;

    /**
     * @brief Default assignment operator.
     */
    gl_program& operator=(const gl_program&) = default;

    /**
     * @brief Move constructor.
     */
    gl_program(gl_program&&) = default;
    gl_program& operator=(gl_program&&) = default;

    ~gl_program()
    {
        release();
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Perform the linking of the program if not already done.
     */
    void link()
    {
        gl_types::uid id = this->id();
#       ifndef MGL_NDEBUG
        assert(id);
#       endif
        if(!gl_object_program::gl_link_status(id))
        {
            gl_object_program::gl_link(id);
            if(!gl_object_program::gl_link_status(id))
                throw gl_link_error(gl_object_program::gl_info_log(id));
            // Link is successfull we can now detach the shaders as recommended here: https://www.opengl.org/wiki/Shader_Compilation.
            else
                detach_shaders();
        }
    }

    /**
     * @brief Call glUseProgram on this program.
     */
    void use() const
    {
        gl_object_program::gl_use(id());
    }

    /**
     * @brief Attach the passed shader to this program.
     * @param p_rhs is the shader to attach
     */
    void attach(const gl_shader& p_rhs)
    {
        this->ensure_created();
        m_attached_shaders[static_cast<std::size_t>(p_rhs.type())] = p_rhs;
        gl_object_program::gl_attach_shader(id(), p_rhs.id());
    }

    /**
     * @brief Detach all the shaders attached to this program.
     */
    void detach_shaders()
    {
        for(auto &shader : m_attached_shaders)
            if(shader)
            {
                gl_object_program::gl_detach_shader(id(), shader.id());
                shader.release();
            }
    }

    /**
     * @brief Create a uniform suitable to use with the set methods below.
     * @param p_name is the name of the uniform.
     * @return Return an object encapsulating the uniform location.
     */
    gl_uniform get_uniform(const char* p_name) const
    {
        return gl_uniform(gl_object_program::gl_uniform_location(id(), p_name));
    }

    /**
     * @brief This method allows to create a vao based on this program.
     * @param p_buffers is the buffers that are gonna be part of this vao.
     * @return Returns the created vao.
     */
    template<typename... T>
    gl_vao make_vao(T&&... p_buffers) const
    {
        return gl_vao(id(), std::forward<T>(p_buffers)...);
    }

    /**
     * @brief Set the passed value to the passed uniform.
     *
     * This is the equivalent to the call to glUniform{1[2|3|4}{i|f}v for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  @code
     *      typedef \/\* your component type \*\/value_type;
     *  @endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mgl::tuple_size meta function for your type.
     * @param p_uniform is the uniform element to set.
     * @param p_valueToBind is the value desired.
     */
    template<typename Data>
    typename std::enable_if<is_vector<Data>::value>::type
    set(gl_uniform p_uniform, Data p_valueToBind) const
    {
        priv::uniform_helper::glm_vector(p_uniform, p_valueToBind);
    }

    /**
     * @brief Set the passed value to the passed uniform.
     *
     * This is the equivalent to the call to glUniformMatrix{2|3|4}fv for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  @code
     *      typedef \/\* your component type \*\/value_type;
     *  @endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mgl::tuple_size meta function for your type.
     * @param p_uniform is the uniform element to set.
     * @param p_valueToBind is the value desired.
     */
    template<typename Data>
    typename std::enable_if<is_matrix<Data>::value>::type
    set(gl_uniform p_uniform, const Data& p_valueToBind) const
    {
        priv::uniform_helper::glm_matrix(p_uniform, std::move(p_valueToBind));
    }

    /**
     * @brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform1i for integer types.
     * @param p_uniform is the uniform element to set.
     * @param p_valueToBind is the value desired.
     */
    template<typename Data>
    typename std::enable_if<std::is_integral<Data>::value>::type
    set(gl_uniform p_uniform, Data p_valueToBind) const
    {
        glUniform1i(p_uniform.id(), p_valueToBind);
    }

    /**
     * @brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform1f for floating point types.
     * @param p_uniform is the uniform element to set.
     * @param p_valueToBind is the value desired.
     */
    template<typename Data>
    typename std::enable_if<std::is_floating_point<Data>::value>::type
    set(gl_uniform p_uniform, Data p_valueToBind) const
    {
        glUniform1f(p_uniform.id(), p_valueToBind);
    }

private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    gl_types::uid gen() override
    {
        return gl_object_program::gl_gen();
    }

    void gl_delete(gl_types::uid p_id)
    {
        gl_object_program::gl_delete(1, &p_id);
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    gl_shader m_attached_shaders[static_cast<std::size_t>(shader_type::Count)];
};


} /* namespace mgl. */

#endif /* GLPROGRAM_HPP_ */
