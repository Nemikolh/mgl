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

/*
 * Forward declaration to get friend working.
 */
// TODO : problem here if size_type is the same as difference_type ( can occurs on some architecture )
// TODO : use SFINAE with is_same to solve the problem here
template<typename T, typename Buff>
typename gl_ptr<T, Buff>::difference_type
operator-(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
gl_ptr<T, Buff> operator+(const gl_ptr<T, Buff>&, typename gl_ptr<T, Buff>::difference_type p_n);
template<typename T, typename Buff>
gl_ptr<T, Buff> operator+(typename gl_ptr<T, Buff>::difference_type p_n, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
gl_ptr<T, Buff> operator+(const gl_ptr<T, Buff>&, typename gl_ptr<T, Buff>::size_type p_n);
template<typename T, typename Buff>
gl_ptr<T, Buff> operator+(typename gl_ptr<T, Buff>::size_type p_n, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
gl_ptr<T, Buff> operator-(const gl_ptr<T, Buff>&, typename gl_ptr<T, Buff>::difference_type p_n);

template<typename T, typename Buff>
bool operator<(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
bool operator>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
bool operator>=(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
bool operator<=(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
template<typename T, typename Buff>
bool operator==(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);


/*
 * Forward declaration for the gl_vector type.
 */
template<typename T, typename Buff = gl_buffer<T>, typename Alloc = gl_allocator<T, Buff> >
class gl_vector;

}  /* namespace gl */
}  /* namespace core */
}  /* namespace nkh */


#endif /* GLFWD_HPP_ */
