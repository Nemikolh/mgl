/*
 * gldata.hpp
 *
 *  Created on: 2 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLDATA_HPP_
#define GLDATA_HPP_

#include "type/gltraits.hpp"
#include "memory/util.hpp"

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

/**
 * Concatenation macro.
 */
#define IMPL_MGL_CAT_I(a, b) a ## b
#define IMPL_MGL_CAT(a, b) IMPL_MGL_CAT_I(a, b)

/**
 * Remove all the element from the given sequence
 */
#define IMPL_MGL_EAT_ALL_AUX_1_END
#define IMPL_MGL_EAT_ALL_AUX_2_END
#define IMPL_MGL_EAT_ALL_AUX_1(a) IMPL_MGL_EAT_ALL_AUX_2
#define IMPL_MGL_EAT_ALL_AUX_2(a) IMPL_MGL_EAT_ALL_AUX_1
#define IMPL_MGL_EAT_ALL(SEQ) \
        IMPL_MGL_CAT(IMPL_MGL_EAT_ALL_AUX_1 SEQ, _END)  \

/**
 * Retrieve the first element of the given sequence.
 */
#define IMPL_MGL_FIRST_AUX(first) \
        first IMPL_MGL_EAT_ALL_AUX_1
#define IMPL_MGL_FIRST(SEQ) \
        IMPL_MGL_CAT(IMPL_MGL_FIRST_AUX SEQ, _END)

/**
 * Eat the first element of a sequence
 */
#define IMPL_MGL_EAT_ONE(first)
#define IMPL_MGL_EAT_FIRST(SEQ) \
        IMPL_MGL_EAT_ONE SEQ

/**
 * Convert the given sequence into nested namespaces.
 */
#define IMPL_MGL_NAMESPACE_START_AUX_1_END
#define IMPL_MGL_NAMESPACE_START_AUX_2_END
#define IMPL_MGL_NAMESPACE_START_AUX_1(elem) \
        namespace elem {               \
        IMPL_MGL_NAMESPACE_START_AUX_2
#define IMPL_MGL_NAMESPACE_START_AUX_2(elem) \
        namespace elem {               \
        IMPL_MGL_NAMESPACE_START_AUX_1

#define IMPL_MGL_NAMESPACE_END_AUX_1_END
#define IMPL_MGL_NAMESPACE_END_AUX_2_END
#define IMPL_MGL_NAMESPACE_END_AUX_1(elem) \
        }               \
        IMPL_MGL_NAMESPACE_END_AUX_2
#define IMPL_MGL_NAMESPACE_END_AUX_2(elem) \
        }               \
        IMPL_MGL_NAMESPACE_END_AUX_1

#define IMPL_MGL_NAMESPACE_START(SEQ)                       \
        IMPL_MGL_CAT(IMPL_MGL_NAMESPACE_START_AUX_1 SEQ, _END)
#define IMPL_MGL_NAMESPACE_END(SEQ)                         \
        IMPL_MGL_CAT(IMPL_MGL_NAMESPACE_END_AUX_1 SEQ, _END)

/**
 * Expand the sequence into a series of namespace lookup.
 */
#define IMPL_MGL_EXPAND_NAMESPACE_AUX_2_END ::
#define IMPL_MGL_EXPAND_NAMESPACE_AUX_1_END ::
#define IMPL_MGL_EXPAND_NAMESPACE_AUX_2(elem) ::elem IMPL_MGL_EXPAND_NAMESPACE_AUX_1
#define IMPL_MGL_EXPAND_NAMESPACE_AUX_1(elem) ::elem IMPL_MGL_EXPAND_NAMESPACE_AUX_2
#define IMPL_MGL_EXPAND_NAMESPACE(SEQ) \
        IMPL_MGL_CAT(IMPL_MGL_EXPAND_NAMESPACE_AUX_1 SEQ, _END)

//#    define BOOST_PP_IF(cond, t, f) BOOST_PP_IIF(BOOST_PP_BOOL(cond), t, f)
//#    define BOOST_PP_IIF(bit, t, f) BOOST_PP_IIF_I(bit, t, f)
//#    define BOOST_PP_IIF_I(bit, t, f) BOOST_PP_IIF_ ## bit(t, f)
//# define BOOST_PP_IIF_0(t, f) f
//# define BOOST_PP_IIF_1(t, f) t
/**
 * BOOST_PP_VARIADIC_SIZE macro renamed to avoid collision.
 */
#define IMPL_MGL_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size
#define IMPL_MGL_VARIADIC_SIZE(...) IMPL_MGL_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#define IMPL_MGL_TUPLE_SIZE(tuple) IMPL_MGL_VARIADIC_SIZE tuple

/**
 * Return the second element in a tuple of three or two attributes.
 */
#define IMPL_MGL_CAPTURE_FIRST_AUX_3(a, b, c) a
#define IMPL_MGL_CAPTURE_SECOND_AUX_3(a, b, c) b
#define IMPL_MGL_CAPTURE_THIRD_AUX_3(a, b, c) c
#define IMPL_MGL_CAPTURE_FIRST_AUX_2(a, b) a
#define IMPL_MGL_CAPTURE_SECOND_AUX_2(a, b) b
#define IMPL_MGL_CAPTURE_THIRD_AUX_2(a, b, c) c
#define IMPL_MGL_CAPTURE_SECOND(ATTRIBUTE) \
        IMPL_MGL_CAT(IMPL_MGL_CAPTURE_SECOND_AUX_, IMPL_MGL_TUPLE_SIZE(ATTRIBUTE)) ATTRIBUTE

#define IMPL_MGL_OFFSET_OF(NAME, ATTRIBUTE) \
    offsetof(NAME, IMPL_MGL_CAPTURE_SECOND(ATTRIBUTE))

#define IMPL_MGL_DEFINE_ATTRIBUTE_OFFSET_AT(R, NAME, INDEX, ATTRIBUTE) \
       namespace mgl {\
            template<>                                                 \
            struct offset_at< \
                        NAME, INDEX>              \
            {                                                          \
                static constexpr std::size_t value = IMPL_MGL_OFFSET_OF(NAME, ATTRIBUTE) ; \
            };                                                         \
        }

#define IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(NAME, ATTRIBUTES)  \
        BOOST_PP_SEQ_FOR_EACH_I_R(                              \
                    1,                                          \
                    IMPL_MGL_DEFINE_ATTRIBUTE_OFFSET_AT,        \
                    NAME,                                       \
                    BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_FILLER_0 ATTRIBUTES,_END))


/**
 * Macro to define attributes data.
 * --------------------------------------------------------------
 * Usage :
 * -------
 *  \code
 *      NKH_DEFINE_GL_ATTRIBUTES(
 *          (namespace0)(namespace1),
 *          attribute_block_name,
 *          (glm::vec3, attribute1)
 *          (int      , attribute2)
 *      )
 *  \endcode
 * Define attributes that can be drawn with a shader program.
 * The attributes name must be in the GLSL vertex shader
 * of the same name than in their declaration. For instance,
 * here the vertex shader would be :
 *  \code
 *      in vec3 attribute1;
 *      in int  attribute2;
 *
 *      ...
 *  \endcode
 * Notes :
 *  - You can't prefix an attribute with "gl_", if you do so the
 *    program will not link. No check is performed on it there, so
 *    it's your job to use it properly.
 */
#define MGL_DEFINE_GL_ATTRIBUTES(NAMESPACE_SEQ, NAME, ATTRIBUTES)                               \
        BOOST_FUSION_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES)                             \
        namespace mgl  {                                                                        \
        namespace priv {                                                                        \
            template<>                                                                          \
            struct is_gl_attributes<                                                            \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME>             \
            {                                                                                   \
                static constexpr bool value = true;                                             \
            };                                                                                  \
        }                                                                                       \
        }                                                                                       \
        IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(                                                   \
                IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME,         \
                ATTRIBUTES)

//                BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME,
/**
 * Macro to adapt existing attributes data.
 * ----------------------------------------------------------------
 * Usage :
 * -------
 *
 *  Same as NKH_DEFINE_GL_ATTRIBUTES but does not have an instrusive
 *  behavior. However whenever the original structure is modified,
 *  you have also to report the modification within the macro
 *  declaration.
 *
 *  Note :
 *    - you can't adapt partially a structure, as computed offset are
 *      perform on the structure plus its sizeof isn't computed there. It
 *      could however maybe be an improvement to the library.
 */
#define MGL_ADAPT_TO_GL_ATTRIBUTES(NAME, ATTRIBUTES)                                            \
        BOOST_FUSION_ADAPT_STRUCT(NAME, ATTRIBUTES)                                             \
        namespace mgl  {                                                                        \
        namespace priv {                                                                        \
            struct is_gl_attributes<NAME>                                                       \
            {                                                                                   \
                static constexpr bool value = true;                                             \
            };                                                                                  \
        }                                                                                       \
        }                                                                                       \
        IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(                                                   \
                NAME,                                                                           \
                ATTRIBUTES)

#endif /* GLDATA_HPP_ */
