/*
 * glutil.hpp
 *
 *  Created on: 25 févr. 2014
 *      Author: nemikolh
 */

#ifndef MGL_UTIL_HPP_
#define MGL_UTIL_HPP_

namespace mgl {

template<typename T, unsigned int N>
struct offset_at
{};

template<typename T, unsigned int N>
struct value_at
{};

template<typename T, unsigned int N>
struct struct_member_name
{};

namespace priv {

template<typename T>
struct seq_size
{};

template<unsigned int N>
struct int_
{
    static constexpr unsigned int value = N;
};

}  /* namespace priv */

}  /* namespace mgl */

#endif /* MGL_UTIL_HPP_ */
