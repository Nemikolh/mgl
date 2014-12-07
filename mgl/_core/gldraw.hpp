/*
 * gldraw.hpp
 *
 *  Created on: 9 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLUTIL_HPP_
#define GLUTIL_HPP_

#include "glfwd.hpp"

namespace mgl {

/**
 * @brief Draw n instance of an object.
 * In order to do so you need to pass at least one buffer with
 *  @code
 *      make_instanced(buffer)
 *  @endcode
 * to the vao. Then the maximum number of object that can be drawn
 * will be less than the minimum size of the buffers marked as
 * instanced.
 * Note :
 *  - Only if MGL_DEBUG is active you'll get a check on primcount,
 *    otherwise you need to provide that parameter carefully.
 * @param p_vao holds both the object data and the per instance data.
 * @param p_primcount is the number of instance to draw.
 */
template<size_t dummy = 0>
void gl_draw_instanced(const gl_vao& p_vao, std::size_t p_primcount);

/**
 * @brief Draw the passed vao.
 * @param p_vao is the vao to draw.
 * @param p_material is the material to use.
 */
template<size_t dummy = 0>
void gl_draw(const gl_vao& p_vao, const gl_program& p_material);

/**
 * @brief Draw the passed vao, assuming a gl_program is already being used.
 * @param p_vao is the vao to draw.
 */
template<size_t dummy = 0>
void gl_draw(const gl_vao& p_vao);

/**
 * @brief Draw the passed data.
 * @param p_data is the attributes data to use.
 * @param p_indices is the array of indices.
 * @param p_material is the shader context.
 */
template<typename T, typename I>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices, const gl_program & p_material);

/**
 * @brief Draw the content.
 * This function does not bind p_data, assuming is has already been bound.
 * @param p_data is the attributes data to use.
 * @param p_indices is the array of indices.
 */
template<typename T, typename I>
void gl_draw(const gl_vector<T> & p_data, const gl_vector<I> & p_indices);



} /* namespace mgl */

#include "gldraw.inl"

#endif /* GLUTIL_HPP_ */
