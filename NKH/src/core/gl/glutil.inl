/*
 * glutil.inl
 *
 *  Created on: 9 juil. 2013
 *      Author: nemikolh
 */

#include "meta/glmetabind.hpp"
#include "meta/gltraits.hpp"

namespace nkh  {
namespace core {
namespace gl   {


/*
 * Implementation details.
*/
template<typename T, typename I, typename M>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices, const gl_shader_context<M> & p_material)
{
    static_assert(priv::is_gl_attributes<T>::value, "The data T must be declared with NKH_DEFINE_GL_ATTRIBUTES.");
    // ------------------------- DECLARE ------------------------ //

    // Loop overs all the attributes of T to bind them to the program.
    gl_bind_attributes<T, M>::map(p_material);

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

    // TODO glDraw
}



} /* namespace gl. */
} /* namespace core. */
} /* namespace nkh. */
