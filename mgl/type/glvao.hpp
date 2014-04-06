/*
 * glvao.hpp
 *
 *  Created on: 19 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLVAO_HPP_
#define GLVAO_HPP_

#include <utility>
#include <type_traits>
#include <tuple>
#include <cassert>
#include "gltraits.hpp"
#include "../meta/glbindbuffer.hpp"

namespace mgl {

struct gl_program;
template<typename T, typename B>
struct gl_vector;

/**
 * @class gl_vao
 * @brief gl_vao enables to use vao in a convenient way.
 * The class offers automatic bindings for attributes, plus
 * the management of the underlying Vertex Array Object(VAO)
 * in a transparent way.
 *
 * Usage :
 * -------
 * This code example illustrate the basic use of this class, plus the way
 * the library has been thought.
 *  @code
 *      // Start with some data :
 *      gl_vector<Vertex> geometry;     // Attribute data per vertex
 *      gl_vector<int>    indices;      // Element data.
 *      gl_vector<int>    ids;          // Attribute data per instance
 *      ...
 *      // Create a program
 *      gl_program my_program;
 *      ...
 *      // create the vao.
 *      gl_vao my_vao = my_program.make_vao(geometry, indices, make_instanced(ids));
 *      ...
 *      // Draw the vao
 *      gl_draw(my_vao, my_material);
 *  @endcode
 */
struct gl_vao
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_vao() = default;
    gl_vao(gl_vao&&) = default;
    const gl_vao& operator=(gl_vao&&) = default;

    gl_vao(const gl_vao&) = delete;
    const gl_vao& operator=(const gl_vao&) = delete;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Bind this vao.
     */
    void bind() const
    {
        if(!m_id)
            throw gl_uninitialized_buffer();
        gl_object_vertexarrays::gl_bind(m_id);
    }

    /**
     * @brief Unbind this vao.
     */
    void unbind() const
    {
        gl_object_vertexarrays::gl_bind(0);
    }

    /**
     * @brief Returns the size of the buffers containing per attribute data.
     * @return Returns the size of the buffers.
     */
    std::size_t size() const
    {
        return m_size;
    }

    /**
     * @brief Returns the size of the instanced arrays.
     * @return Returns the size of the instanced arrays.
     */
    std::size_t size_instanced() const
    {
        return m_size_instanced;
    }

    /**
     * @brief Returns the elements type needed for the calls to gl_draws.
     * @return Returns the elements type.
     */
    GLenum elements_type() const
    {
        return m_elements_type;
    }

private:

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Constructor.
     * @param p_vs is the list of gl_vector that we want to associate with this vao.
     */
    template<typename... Arg>
    gl_vao(gl_types::id p_program_id, Arg&&... p_vs)
        : m_id{0}
        , m_elements_type{0}
        , m_size{0}
        , m_size_instanced{0}
    {
        unpack(p_program_id, std::forward<Arg>(p_vs)...);
    }

    // ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

    friend gl_program;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    template<typename... Arg>
    void unpack(gl_types::id p_program_id, Arg&&... p_args)
    {
        // Generate a new vao object and bind it :
        gl_object_vertexarrays::gl_gen(1, &m_id);
        bind();
        // Bind the attributes for each buffer
        gl_bind_buffers binder(p_program_id);
        std::tie(m_elements_type, m_size, m_size_instanced) = binder.map(std::forward<Arg>(p_args)...);
        // Unbind the vao.
        unbind();
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The vao id. */
    gl_types::id m_id;
    /** The type of the integers. */
    gl_types::en m_elements_type;
    /** The size of the buffers. */
    std::size_t  m_size;
    /** The size of the instanced arrays. */
    std::size_t  m_size_instanced;
};

} /* namespace mgl. */

#endif /* GLVAO_HPP_ */
