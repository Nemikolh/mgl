/*
 * glbinder.hpp
 *
 *  Created on: 5 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLBINDER_HPP_
#define GLBINDER_HPP_

#include "meta/gltraits.hpp"

namespace nkh {
namespace core {
namespace gl {

/**
 * \class gl_attribute_binder is a functor that encapsulate the OpenGL call binding
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
                    GLenum          p_component_type)
    {
        gl_types::id attribute_id;
        // ------------------------- DECLARE ------------------------ //

        attribute_id = glGetAttribLocation(&m_program_id, p_attribute_name);
        if(attribute_id != -1)
        {
            glVertexAttribPointer(attribute_id, p_nb_component, p_component_type, GL_FALSE, p_stride, p_offset);
            glEnableVertexAttribArray(attribute_id);
        }
    }

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_attribute_binder()
        : m_program_id{0}
    {}

private:
    /** The program id currently bound. */
    gl_types::id    m_program_id;
};

} /* namespacce nkh. */
} /* namespace core. */
} /* namespace gl. */

#endif /* GLBINDER_HPP_ */
