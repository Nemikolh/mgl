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
#include <array>
#include "glvao.hpp"
#include "../meta/gluniform.hpp"
#include "gltraits.hpp"

namespace mgl {

/**
 * \class gl_program is a wrapper for an opengl shader program.
 */
struct gl_program
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Default constructor.
     */
    gl_program() noexcept
        : m_program_id(0)
    {}

    /**
     * @brief Copy isn't allowed.
     */
    gl_program(const gl_program& p_rhs) = delete;

    /**
     * @brief Move constructor.
     * @param p_rhs is the temporary.
     */
    gl_program(gl_program&& p_rhs) noexcept
        : m_program_id(p_rhs.m_program_id)
    {
        p_rhs.m_program_id = 0;
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Attach the passed shader to this program.
     * @param p_rhs is the shader to attach
     */
    void attach(const gl_shader& p_rhs) const
    {
        m_attached_shaders[static_cast<std::size_t>(p_rhs.type())] = p_rhs;
        gl_object_program::gl_attach_shader(m_program_id, p_rhs.id());
    }

    gl_shader get(shader_type p_type) const
    {
#       ifndef MGL_NDEBUG
        assert(p_type != shader_type::Count);
#       endif
        return m_attached_shaders[static_cast<std::size_t>(p_type)];
    }

    /**
     * \brief This method allows to create a vao based on this program.
     * \param p_buffers is the buffers that are gonna be part of this vao.
     * \return Returns the created vao.
     */
    template<typename... T>
    gl_vao make_vao(T&&... p_buffers)
    {
        ensure_linked();
        return gl_vao(id(), std::forward<T>(p_buffers)...);
    }

    /**
     * \brief Returns the id of this program.
     * \return Returns the opengl id.
     */
    gl_types::id id() const
    {
        return m_program_id;
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform{1[2|3|4}{i|f}v for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mgl::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<is_vector<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        priv::uniform_helper::glm_vector(p_uniform, p_valueToBind);
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniformMatrix{2|3|4}fv for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mlg::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<is_matrix<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        priv::uniform_helper::glm_matrix(p_uniform, std::move(p_valueToBind));
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform{i|f} for arithmetic types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mlg::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<std::is_arithmetic<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        // TODO
    }

private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Perform the linking of the program if note already done.
     */
    void ensure_linked()
    {
#       ifndef MGL_NDEBUG
        assert(m_program_id);
#       endif
        gl_object_program::gl_link(m_program_id);
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    gl_types::id m_program_id;
    gl_shader m_attached_shaders[static_cast<std::size_t>(shader_type::Count)];
};


} /* namespace mgl. */

#endif /* GLPROGRAM_HPP_ */
