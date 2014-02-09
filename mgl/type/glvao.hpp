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
#include <cassert>
#include "meta/gltraits.hpp"


namespace nkh {
namespace core {
namespace gl {

struct gl_program;
template<typename T>
struct gl_vector;

/**
 * \class gl_vao
 * \brief gl_vao enables to use vao in a convenient way.
 * The class offers automatic bindings for attributes, plus
 * the management of the underlying Vertex Array Object(VAO)
 * in a transparent way.
 *
 * Usage :
 * -------
 * This code example illustrate the basic use of this class, plus the way
 * the library has been thought.
 *  \code
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
 *  \endcode
 */
struct gl_vao
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Bind this vao.
     */
    void bind() const
    {
        gl_object_vertexarrays::gl_bind(m_id);
    }

    /**
     * \brief Unbind this vao.
     */
    void unbind() const
    {
        gl_object_vertexarrays::gl_bind(0);
    }

    /**
     * \brief Returns the size of the buffers containing per attribute data.
     * \return Returns the size of the buffers.
     */
    std::size_t size() const
    {
        return m_size;
    }

    /**
     * \brief Returns the elements type.
     * \return Returns the elements type.
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
     * \brief Constructor.
     * \param p_vs is the list of gl_vector that we want to associate with this vao.
     */
    template<typename... Arg>
    gl_vao(gl_types::id p_program_id, Arg&&... p_vs)
        : m_id{0}
        , m_elements_type{0}
        , m_size{0}
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
        pass(bindBuffer(p_program_id, std::forward<Arg>(p_args))...);
        // Unbind the vao.
        unbind();
    }

    template<typename... Args>
    void pass(Args&&...)
    {}

    // TODO : don't test if it is integral but if it is an attribute buffer.
    template<typename T>
    typename std::enable_if<!std::is_integral<T>::value>::type
    bindBuffer(gl_types::id p_program_id, const gl_vector<T>& p_buffer)
    {
        p_buffer.bind();
        gl_attribute_binder binder{p_program_id};
        gl_bind_attributes<T>::map(binder);
#ifndef NKH_NDEBUG
        assert(m_size == 0 || m_size == p_buffer.size());
#endif
        m_size = p_buffer.size();
    }

    // TODO : same apply here
    template<typename I>
    typename std::enable_if<std::is_integral<I>::value>::type
    bindBuffer(gl_types::id p_program_id, const gl_vector<I>& p_buffer)
    {
        // Bind the element buffer.
        p_buffer.bind();
        m_elements_type = gl_enum_type<I>::value;
    }

    template<typename T>
    void
    bindBuffer(gl_types::id p_program_id, const gl_instanced<T>& p_buffer)
    {
        p_buffer.bind();
        gl_attribute_binder binder{p_program_id, p_buffer.get_divisor()};
        gl_bind_attributes<T>::map(binder);
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
};

} /* namespace gl. */
} /* namespace core. */
} /* namespace nkh. */


#endif /* GLVAO_HPP_ */
