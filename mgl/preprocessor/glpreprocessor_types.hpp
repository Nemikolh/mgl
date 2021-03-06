/*
 * glpreprocessor_types.hpp
 *
 *  Created on: 6 mai 2014
 *      Author: nemikolh
 */

#ifndef GLPREPROCESSOR_TYPES_HPP_
#define GLPREPROCESSOR_TYPES_HPP_

#include "glpreprocessor_base.hpp"

/**
 * Cast to string the passed parameter.
 */
#define IMPL_MGL_TO_STR(arg) IMPL_MGL_TO_STR_I(arg)
#define IMPL_MGL_TO_STR_I(arg) #arg

/**
 * Retrieve the first element of the given sequence.
 */
#define IMPL_MGL_SEQ_HEAD(seq) IMPL_MGL_SEQ_HEAD_I(seq)
#define IMPL_MGL_SEQ_HEAD_I(seq) IMPL_MGL_SEQ_HEAD_II(IMPL_MGL_SEQ_HEAD_0 seq)
#define IMPL_MGL_SEQ_HEAD_II(im) IMPL_MGL_SEQ_HEAD_III(im)
#define IMPL_MGL_SEQ_HEAD_III(x, _) x
#define IMPL_MGL_SEQ_HEAD_0(x) x, IMPL_MGL_NIL

/**
 * Eat the first element of a sequence
 */
#define IMPL_MGL_SEQ_TAIL(seq) IMPL_MGL_SEQ_TAIL_I seq
#define IMPL_MGL_SEQ_TAIL_I(x)

/**
 * Return the 1st/2nd/3rd element in a tuple of three or two attributes.
 */
#define IMPL_MGL_CAPTURE_FIRST_AUX_3(a, b, c) a
#define IMPL_MGL_CAPTURE_SECOND_AUX_3(a, b, c) b
#define IMPL_MGL_CAPTURE_THIRD_AUX_3(a, b, c) c
#define IMPL_MGL_CAPTURE_FIRST_AUX_2(a, b) a
#define IMPL_MGL_CAPTURE_SECOND_AUX_2(a, b) b
#define IMPL_MGL_CAPTURE_THIRD_AUX_2(a, b)

#define IMPL_MGL_CAPTURE_FIRST(ATTRIBUTE) \
        IMPL_MGL_CAT(IMPL_MGL_CAPTURE_FIRST_AUX_, IMPL_MGL_TUPLE_SIZE(ATTRIBUTE)) ATTRIBUTE
#define IMPL_MGL_CAPTURE_SECOND(ATTRIBUTE) \
        IMPL_MGL_CAT(IMPL_MGL_CAPTURE_SECOND_AUX_, IMPL_MGL_TUPLE_SIZE(ATTRIBUTE)) ATTRIBUTE
#define IMPL_MGL_CAPTURE_THIRD(ATTRIBUTE) \
        IMPL_MGL_CAT(IMPL_MGL_CAPTURE_THIRD_AUX_, IMPL_MGL_TUPLE_SIZE(ATTRIBUTE)) ATTRIBUTE

/**
 * Returns the size of the sequence.
 */
#define IMPL_MGL_SEQ_SIZE(seq) IMPL_MGL_CAT(IMPL_MGL_SEQ_SIZE_0 seq, _END)

#define IMPL_MGL_SEQ_SIZE_0(_) IMPL_MGL_SEQ_SIZE_1
#define IMPL_MGL_SEQ_SIZE_1(_) IMPL_MGL_SEQ_SIZE_2
#define IMPL_MGL_SEQ_SIZE_2(_) IMPL_MGL_SEQ_SIZE_3
#define IMPL_MGL_SEQ_SIZE_3(_) IMPL_MGL_SEQ_SIZE_4
#define IMPL_MGL_SEQ_SIZE_4(_) IMPL_MGL_SEQ_SIZE_5
#define IMPL_MGL_SEQ_SIZE_5(_) IMPL_MGL_SEQ_SIZE_6
#define IMPL_MGL_SEQ_SIZE_6(_) IMPL_MGL_SEQ_SIZE_7
#define IMPL_MGL_SEQ_SIZE_7(_) IMPL_MGL_SEQ_SIZE_8
#define IMPL_MGL_SEQ_SIZE_8(_) IMPL_MGL_SEQ_SIZE_9

#define IMPL_MGL_SEQ_SIZE_0_END 0
#define IMPL_MGL_SEQ_SIZE_1_END 1
#define IMPL_MGL_SEQ_SIZE_2_END 2
#define IMPL_MGL_SEQ_SIZE_3_END 3
#define IMPL_MGL_SEQ_SIZE_4_END 4
#define IMPL_MGL_SEQ_SIZE_5_END 5
#define IMPL_MGL_SEQ_SIZE_6_END 6
#define IMPL_MGL_SEQ_SIZE_7_END 7
#define IMPL_MGL_SEQ_SIZE_8_END 8
#define IMPL_MGL_SEQ_SIZE_9_END 9

/**
 * BOOST_PP_VARIADIC_SIZE macro renamed to avoid collision.
 */
#define IMPL_MGL_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size
#define IMPL_MGL_VARIADIC_SIZE(...) IMPL_MGL_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#define IMPL_MGL_TUPLE_SIZE(tuple) IMPL_MGL_VARIADIC_SIZE tuple

/**
 * Return the size of the head tuple in the passed sequence.
 */

#define IMPL_MGL_SEQ_HEAD_SIZE_AUX_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size,
#define IMPL_MGL_SEQ_HEAD_SIZE_AUX(...) IMPL_MGL_SEQ_HEAD_SIZE_AUX_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)

#define IMPL_MGL_SEQ_HEAD_SIZE(seq) \
        IMPL_MGL_SEQ_HEAD_SIZE_I(IMPL_MGL_SEQ_HEAD_SIZE_AUX seq)
#define IMPL_MGL_SEQ_HEAD_SIZE_I(im) IMPL_MGL_SEQ_HEAD_SIZE_II(im)
#define IMPL_MGL_SEQ_HEAD_SIZE_II(size, ...) size

/**
 * Wrap tuple element of a sequence into an englobing element.
 */
#define IMPL_MGL_FILLER_3_0(X, Y, Z)                             \
        ((X, Y, Z)) IMPL_MGL_FILLER_3_1
#define IMPL_MGL_FILLER_3_1(X, Y, Z)                             \
        ((X, Y, Z)) IMPL_MGL_FILLER_3_0
#define IMPL_MGL_FILLER_3_0_END
#define IMPL_MGL_FILLER_3_1_END

#define IMPL_MGL_FILLER_2_0(X, Y)                                \
        ((X, Y)) IMPL_MGL_FILLER_2_1
#define IMPL_MGL_FILLER_2_1(X, Y)                                \
        ((X, Y)) IMPL_MGL_FILLER_2_0
#define IMPL_MGL_FILLER_2_0_END
#define IMPL_MGL_FILLER_2_1_END

#define IMPL_MGL_FILLER_AUX(seq, tuple_size) IMPL_MGL_FILLER_AUX_I(seq, tuple_size)
#define IMPL_MGL_FILLER_AUX_I(seq, tuple_size) \
        IMPL_MGL_CAT(IMPL_MGL_FILLER_ ## tuple_size ## _0 seq,_END)

#define IMPL_MGL_FILLER(seq) \
        IMPL_MGL_FILLER_AUX(seq, IMPL_MGL_SEQ_HEAD_SIZE(seq))

//IMPL_MGL_FILLER((1,23)(ab,c))

#endif /* GLPREPROCESSOR_TYPES_HPP_ */
