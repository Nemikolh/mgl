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



#define IMPL_MGL_CAPTURE_SECOND(ATTRIBUTE) \
        BOOST_PP_TUPLE_ELEM(2, 1, ATTRIBUTE)

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
                BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME,         \
                ATTRIBUTES)


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

///**
// * Macro to define uniform data.
// * --------------------------------------------------------------
// * Usage :
// * -------
// *
// * Same as NKH_DEFINE_GL_ATTRIBUTES
// */
//#define NKH_DEFINE_GL_UNIFORM BOOST_FUSION_DEFINE_STRUCT


#endif /* GLDATA_HPP_ */
