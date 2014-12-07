/*
 * glpreprocessor_control.hpp
 *
 *  Created on: 7 mai 2014
 *      Author: nemikolh
 */

#ifndef GLPREPROCESSOR_CONTROL_HPP_
#define GLPREPROCESSOR_CONTROL_HPP_

#include "glpreprocessor_base.hpp"

/**
 * Increment an integer.
 */
#define IMPL_MGL_INC(i) IMPL_MGL_CAT(IMPL_MGL_INC_, i)
#define IMPL_MGL_INC_0 1
#define IMPL_MGL_INC_1 2
#define IMPL_MGL_INC_2 3
#define IMPL_MGL_INC_3 4
#define IMPL_MGL_INC_4 5
#define IMPL_MGL_INC_5 6
#define IMPL_MGL_INC_6 7
#define IMPL_MGL_INC_7 8
#define IMPL_MGL_INC_8 9

/**
 * Decrement an integer.
 */
#define IMPL_MGL_DEC(i) IMPL_MGL_CAT(IMPL_MGL_DEC_, i)
#define IMPL_MGL_DEC_0 0
#define IMPL_MGL_DEC_1 0
#define IMPL_MGL_DEC_2 1
#define IMPL_MGL_DEC_3 2
#define IMPL_MGL_DEC_4 3
#define IMPL_MGL_DEC_5 4
#define IMPL_MGL_DEC_6 5
#define IMPL_MGL_DEC_7 6
#define IMPL_MGL_DEC_8 7
#define IMPL_MGL_DEC_9 8

/**
 * Loop over a sequence applying macro on each attribute.
 * the macro receive the following parameter:
 *          MACRO(elem, index, data)
 * where data is a global data passed to the function.
 */
#define IMPL_MGL_FOR_EACH(seq, macro, data) \
        IMPL_MGL_FOR_EACH_I(seq, IMPL_MGL_SEQ_SIZE(seq), macro, data)
#define IMPL_MGL_FOR_EACH_I(seq, size, macro, data) \
        IMPL_MGL_CAT(IMPL_MGL_FOR_EACH_I_, size)(seq, macro, 0, data)
#define IMPL_MGL_FOR_EACH_I_9(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_8(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_8(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_7(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_7(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_6(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_6(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_5(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_5(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_4(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_4(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_3(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_3(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_2(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_2(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_1(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_1(seq, macro, i, data) macro(IMPL_MGL_SEQ_HEAD(seq), i, data) IMPL_MGL_FOR_EACH_I_0(IMPL_MGL_SEQ_TAIL(seq), macro, IMPL_MGL_INC(i), data)
#define IMPL_MGL_FOR_EACH_I_0(seq, macro, i, data)


#endif /* GLPREPROCESSOR_CONTROL_HPP_ */
