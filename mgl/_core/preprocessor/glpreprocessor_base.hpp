/*
 * glpreprocessor_base.hpp
 *
 *  Created on: 18 mai 2014
 *      Author: nemikolh
 */

#ifndef GLPREPROCESSOR_BASE_HPP_
#define GLPREPROCESSOR_BASE_HPP_


/**
 * Concatenation macro.
 */
#define IMPL_MGL_CAT_I(a, b) a ## b
#define IMPL_MGL_CAT(a, b) IMPL_MGL_CAT_I(a, b)

/**
 * Convert an integer into a boolean.
 */
#define IMPL_MGL_BOOL(i) IMPL_MGL_CAT(IMPL_MGL_BOOL_, i)
#define IMPL_MGL_BOOL_0 0
#define IMPL_MGL_BOOL_1 1
#define IMPL_MGL_BOOL_2 1
#define IMPL_MGL_BOOL_3 1
#define IMPL_MGL_BOOL_4 1
#define IMPL_MGL_BOOL_5 1
#define IMPL_MGL_BOOL_6 1
#define IMPL_MGL_BOOL_7 1
#define IMPL_MGL_BOOL_8 1
#define IMPL_MGL_BOOL_9 1

/**
 * If macro.
 */
#define IMPL_MGL_IF(cond, t, f) IMPL_MGL_IF_I(IMPL_MGL_BOOL(cond), t, f)
#define IMPL_MGL_IF_I(bit, t, f) IMPL_MGL_CAT(IMPL_MGL_IF_I_, bit(t, f))
#define IMPL_MGL_IF_I_0(t,f) f
#define IMPL_MGL_IF_I_1(t,f) t

#endif /* GLPREPROCESSOR_BASE_HPP_ */
