/*
 * gldata.hpp
 *
 *  Created on: 2 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLDATA_HPP_
#define GLDATA_HPP_

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include "meta/gltraits.hpp"



#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define SEQ (glm::vec3, position)(glm::vec3, normal)(glm::vec2, uvcoord)
#define EAT_FIRST_2_END
#define EAT_FIRST_1_END
#define EAT_FIRST_1(a, b) (b) EAT_FIRST_2
#define EAT_FIRST_2(a, b) (b) EAT_FIRST_1
#define EAT_FIRST(seq) BOOST_PP_CAT(EAT_FIRST_1 seq, _END)
#define TEST(r, data, b) data b = r - 2 ;

BOOST_PP_SEQ_FOR_EACH(TEST, constexpr std::uint8_t, EAT_FIRST(SEQ)) // expands to w_ x_ y_ z_

namespace nkh  {
namespace core {
namespace gl   {
namespace priv {
template<typename T>
struct in_impl;
}
}
}
}
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
#define NKH_DEFINE_GL_ATTRIBUTES(NAMESPACE_SEQ, NAME, ATTRIBUTES)             \
        BOOST_FUSION_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES)    \
        namespace nkh  {                                               \
        namespace core {                                               \
        namespace gl   {                                               \
        namespace priv {                                               \
            template<>                                                 \
            struct is_gl_attributes<                                         \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME>              \
            {                                                          \
                static constexpr bool value = true;                    \
            };                                                         \
                                                                       \
            template<>                                                 \
            struct in_impl< \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME>              \
            {                                                          \
            };                                                         \
        }                                                              \
        }                                                              \
        }                                                              \
        }


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
#define NKH_ADAPT_TO_GL_ATTRIBUTES(NAMESPACE_SEQ, NAME, ATTRIBUTES)     \
        BOOST_FUSION_ADAPT_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES)      \
        namespace nkh  {                                                \
        namespace core {                                                \
        namespace gl   {                                                \
        namespace priv {                                                \
            struct is_gl_attributes<                                    \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME>              \
            {                                                           \
                static constexpr bool value = true;                     \
            };                                                          \
        }                                                               \
        }                                                               \
        }                                                               \
        }

/**
 * Macro to define uniform data.
 * --------------------------------------------------------------
 * Usage :
 * -------
 *
 * Same as NKH_DEFINE_GL_ATTRIBUTES
 */
#define NKH_DEFINE_GL_UNIFORM BOOST_FUSION_DEFINE_STRUCT


#endif /* GLDATA_HPP_ */
