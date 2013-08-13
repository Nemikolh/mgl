/*
 * gldata.hpp
 *
 *  Created on: 2 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLDATA_HPP_
#define GLDATA_HPP_

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include "gltraits.hpp"

/**
 * Macro to define attributes data.
 * --------------------------------------------------------------
 * Usage :
 * -------
 *
 *      NKH_DEFINE_GL_ATTRIBUTES(
 *          (namespace0)(namespace1),
 *          attribute_block_name,
 *          (glm::vec3, attribute1)
 *          (int      , attribute2)
 *      )
 *
 * Define attributes that can be drawn with a shader program.
 * The attributes name must be in the GLSL vertex shader
 * of the same name than in their declaration. For instance,
 * here the vertex shader would be :
 *
 *      in vec3 attribute1;
 *      in int  attribute2;
 *
 *      ...
 *
 *
 */
#define NKH_DEFINE_GL_ATTRIBUTES(NAMESPACE_SEQ, NAME, ATTRIBUTES)             \
        BOOST_FUSION_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES)    \
        namespace nkh  {                                               \
        namespace core {                                               \
        namespace gl   {                                               \
        namespace priv {                                               \
            struct is_gl_attributes<                                         \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME>              \
            {                                                          \
                static constexpr bool value = true;                    \
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
 *      you can't adapt partially a structure, as computed offset are
 *  perform on the structure plus its sizeof isn't computed there. It
 *  could however maybe be an improvement to the library.
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
