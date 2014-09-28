/*
 * glbinder.hpp
 *
 *  Created on: 5 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLBINDER_HPP_
#define GLBINDER_HPP_

#include "../type/gltraits.hpp"

namespace mgl {

/**
 * @ingroup attributes
 * @brief gl_attribute_binder is a functor that encapsulate the OpenGL call binding
 *          an attribute to a shader.
 */
struct gl_attribute_binder
{
    // ================================================================ //
    // ============================ FUNCTOR =========================== //
    // ================================================================ //

    /**
     * Bind the passed attribute.
     * @param p_attribute_name is the name of the attribute to bind.
     * @param p_nb_component is the number of component  for the attribute.
     * @param p_offset is the offset where the attribute start in the buffer.
     * @param p_stride is the stride between two consecutives values.
     * @param p_component_type is the OpenGL type of each component.
     */
    void operator()(char const*     p_attribute_name,
                    int             p_nb_component,
                    std::size_t     p_offset,
                    std::size_t     p_stride,
                    GLenum          p_component_type) const
    {
        GLint attribute_id;
        // ------------------------- DECLARE ------------------------ //

        attribute_id = glGetAttribLocation(m_program_id, p_attribute_name);

        // The attribute id can -1 if the attribute is not used inside the shader
        // or if the attribute start with the reserved prefix "gl_"
        if(attribute_id != -1)
        {
            glVertexAttribPointer(attribute_id, p_nb_component, p_component_type, GL_FALSE, p_stride,
                                  reinterpret_cast<const void*>(p_offset));
            glEnableVertexAttribArray(attribute_id);
            glVertexAttribDivisor(attribute_id, m_divisor);
        }
    }

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default constructor.
     */
    gl_attribute_binder()
        : m_divisor{0}
        , m_program_id{0}
    {}

    /**
     * \brief Proper constructor.
     * @param p_program_id is the id of the gl_program.
     * @param p_divisor is the divisor for the attribute.
     */
    gl_attribute_binder(gl_types::id p_program_id, char p_divisor = 0)
        : m_divisor{p_divisor}
        , m_program_id{p_program_id}
    {}

    /**
     * \brief Copy constructor.
     */
    gl_attribute_binder(const gl_attribute_binder&) = default;


private:
    /** The attribute divisor parameter. */
    char            m_divisor;
    /** The program id currently bound. */
    gl_types::id    m_program_id;
};

} /* namespacce mgl */

#endif /* GLBINDER_HPP_ */
