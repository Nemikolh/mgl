/*
 * glfwd.hpp
 *
 *  Created on: 27 oct. 2013
 *      Author: nemikolh
 */

#ifndef GLFWD_HPP_
#define GLFWD_HPP_

namespace mgl {

/*
 * Forward declaration.
 */
template<typename T>
struct gl_buffer_type;

template<typename T, typename Container, typename Buff = gl_buffer_type<T>>
struct gl_allocator;

template<typename T, typename Buff = gl_buffer_type<T>>
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

template<typename T, typename C1, typename B1, typename U, typename C2, typename B2>
bool operator==(const gl_allocator<T, C1, B1>& p_a, const gl_allocator<U, C2, B2>& p_b);

/* Forward declaration for the gl_vector type. */
template<typename T, typename Buff = gl_buffer_type<T>>
class gl_vector;

/* Forward declaration of gl_vao. */
struct gl_vao;

/* Forward declaration fo gl_program. */
struct gl_program;

/* Forward declaration of gl_simple_buffer. */
template<typename T, typename B>
class gl_simple_buffer;

/* Forward declaration of gl_instanced. */
template<typename T>
struct gl_instanced;


}  /* namespace mgl */


#endif /* GLFWD_HPP_ */
