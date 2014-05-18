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
    typedef int_            type;
    typedef unsigned int    value_type;

    static constexpr unsigned int value = N;
};

template<bool B, typename T, typename F>
struct eval_if_c
{
    typedef T type;
};

template<typename T, typename F>
struct eval_if_c<false, T, F>
{
    typedef F type;
};

template<typename Cond, typename T, typename F>
struct eval_if
{
    typedef typename eval_if_c<Cond::value, T, F>::type _f;
    typedef typename _f::type type;
};

template<typename N, typename D>
struct divides
{
    typedef int_<N::value / D::value> type;
    static constexpr unsigned int value = type::value;
};

}  /* namespace priv */

}  /* namespace mgl */

#endif /* MGL_UTIL_HPP_ */
