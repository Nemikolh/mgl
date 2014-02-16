/*
 * glutil.inl
 *
 *  Created on: 9 juil. 2013
 *      Author: nemikolh
 */

#include "meta/glmetabind.hpp"
#include "type/gltraits.hpp"
#include "glvao.hpp"
#include "glprogram.hpp"
#include "glvector.hpp"

namespace mgl {

/*
 * Implementation details.
 */
void gl_draw_instanced(const gl_vao& p_vao, std::size_t p_primcount)
{
    // ------------------------- DECLARE ------------------------ //

    // Bind the  vao.
    p_vao.bind();

    // TODO perform the check on primcount in debug mode.
    glDrawElementsInstanced(GL_TRIANGLES, p_vao.size(), p_vao.elements_type(), 0, p_primcount);
}

/*
 * Implementation details.
*/
template<typename T, typename I>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices, const gl_program & p_program)
{
    static_assert(priv::is_gl_attributes<T>::value, "The data T must be declared with NKH_DEFINE_GL_ATTRIBUTES.");
    // ------------------------- DECLARE ------------------------ //

    // Bind the data.
    p_data.bind();

    // Loop overs all the attributes of T to bind them to the program.
    gl_bind_attributes<T>::map(gl_attribute_binder{p_program.id()});

    // Call to the draw
    gl_draw(p_data, p_indices);
}

/*
 * Implementation details.
*/
template<typename T, typename I>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices)
{
    static_assert(std::is_integral<I>::value, "Indices must be integral types, unsigned recommended.");
    // ------------------------- DECLARE ------------------------ //

    // Bind the element buffer.
    p_indices.bind();

    glDrawElements(GL_TRIANGLES, p_data.size(), gl_enum_from_type<I>::value, 0);
}

/*
 * Implementation details
 */
void gl_draw(const gl_vao& p_vao)
{
    // ------------------------- DECLARE ------------------------ //

    // Bind the vao.
    p_vao.bind();

    glDrawElements(GL_TRIANGLES, p_vao.size(), p_vao.elements_type(), 0);
}

} /* namespace mgl */
