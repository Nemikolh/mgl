/*
 * glfwd.hpp
 *
 *  Created on: 27 oct. 2013
 *      Author: nemikolh
 */

#ifndef GLFWD_HPP_
#define GLFWD_HPP_

#include "glrequires.hpp"

namespace nkh {
namespace core {
namespace gl {

/*
 * Forward declaration.
 */
template<typename T, typename Buff = gl_buffer<T>>
struct gl_allocator;

template<typename T, typename Buff = gl_buffer<T>>
struct gl_ptr;

template<typename T, typename Buff = gl_buffer<T>, typename Alloc = gl_allocator<T, Buff> >
class gl_vector;

}  /* namespace gl */
}  /* namespace core */
}  /* namespace nkh */


#endif /* GLFWD_HPP_ */
