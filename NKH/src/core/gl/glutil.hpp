/*
 * glutil.hpp
 *
 *  Created on: 9 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLUTIL_HPP_
#define GLUTIL_HPP_

#include "gldata.hpp"
#include <type_traits>

namespace nkh  {
namespace core {
namespace gl   {

/**
 * \brief Draw the passed data.
 * \param p_data is the attributes data to use.
 * \param p_indices is the array of indices.
 * \param p_material is the shader context.
 */
template<typename T, typename I, typename M>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices, const gl_shader_context<M> & p_material);

/**
 * \brief Draw the content.
 * \param p_data is the attributes data to use.
 * \param p_indices is the array of indices.
 */
template<typename T, typename I>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices);

} /* namespace gl. */
} /* namespace core. */
} /* namespace nkh. */

#include "glutil.hpp"

#endif /* GLUTIL_HPP_ */
