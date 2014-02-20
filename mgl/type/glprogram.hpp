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
    gl_program()
        : m_program_id(0)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

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
};


} /* namespace mgl. */

#endif /* GLPROGRAM_HPP_ */
