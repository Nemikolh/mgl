/*
 * gldata.hpp
 *
 *  Created on: 2 juil. 2013
 *      Author: nemikolh
 */
/** @file */

#ifndef GLDATA_HPP_
#define GLDATA_HPP_

#include "type/gltraits.hpp"
#include "meta/glutil.hpp"

#include "preprocessor/glpreprocessor_types.hpp"
#include "preprocessor/glpreprocessor_control.hpp"

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

// --------------------------------------------------------------------------------- //

/**
 * Add the offset_at<> template specialization for an attribute.
 */
#define IMPL_MGL_OFFSET_OF(NAME, ATTRIBUTE) \
    offsetof(NAME, IMPL_MGL_CAPTURE_SECOND(ATTRIBUTE))
#define IMPL_MGL_DEFINE_ATTRIBUTE_OFFSET_AT(ATTRIBUTE, INDEX, NAME) \
            template<>                                                 \
            struct offset_at< \
                        NAME, INDEX>              \
            {                                                          \
                static constexpr std::size_t value = IMPL_MGL_OFFSET_OF(NAME, ATTRIBUTE) ; \
            };

/**
 * Add the offset_at<> template specialization for the attributes.
 */
#define IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(NAME, ATTRIBUTES)  \
        IMPL_MGL_FOR_EACH(                                      \
                    IMPL_MGL_FILLER(ATTRIBUTES),                \
                    IMPL_MGL_DEFINE_ATTRIBUTE_OFFSET_AT,        \
                    NAME)

// --------------------------------------------------------------------------------- //

/**
 * Add the member name definition for an attribute.
 */
#define IMPL_MGL_DEFINE_MEMBER_NAME(MEMBER_NAME, INDEX, STRUCT_NAME)\
template<>                                                          \
struct struct_member_name<STRUCT_NAME, INDEX>                       \
{                                                                   \
    typedef char const* type;                                       \
                                                                    \
    static type call()                                              \
    { return IMPL_MGL_TO_STR(IMPL_MGL_CAPTURE_SECOND(MEMBER_NAME)); }\
};

#define IMPL_MGL_DEFINE_MEMBERS_NAMES(NAME, ATTRIBUTES) \
        IMPL_MGL_FOR_EACH(                              \
                    IMPL_MGL_FILLER(ATTRIBUTES),        \
                    IMPL_MGL_DEFINE_MEMBER_NAME,        \
                    NAME)

// --------------------------------------------------------------------------------- //

/**
 * Add the value_at<> template specialization for the attributes.
 */
#define IMPL_MGL_DEFINE_ATTRIBUTE_VALUE_AT(ATTRIBUTE, INDEX, NAME)    \
        template<>                                          \
        struct value_at<NAME, INDEX>                        \
        {                                                   \
            typedef IMPL_MGL_CAPTURE_FIRST(ATTRIBUTE) type; \
        };

#define IMPL_MGL_DEFINE_ATTRIBUTES_VALUE_AT(NAME, ATTRIBUTES)   \
        IMPL_MGL_FOR_EACH(                                      \
                    IMPL_MGL_FILLER(ATTRIBUTES),                \
                    IMPL_MGL_DEFINE_ATTRIBUTE_VALUE_AT,         \
                    NAME)

// --------------------------------------------------------------------------------- //

/**
 * Add the members definition inside the class.
 */
#define IMPL_MGL_DEFINE_MEMBER(MEMBER, INDEX, _) \
        IMPL_MGL_CAPTURE_FIRST(MEMBER) IMPL_MGL_CAPTURE_SECOND(MEMBER);

#define IMPL_MGL_DEFINE_MEMBERS(ATTRIBUTES) \
        IMPL_MGL_FOR_EACH(                  \
                    IMPL_MGL_FILLER(ATTRIBUTES),\
                    IMPL_MGL_DEFINE_MEMBER,     \
                    _)

// --------------------------------------------------------------------------------- //

/**
 * Set gl_attributes to true for a given type.
 */
#define IMPL_MGL_DEFINE_IS_GL_ATTRIBUTE(NAME)   \
        template<>                              \
        struct is_gl_attributes<NAME>           \
        {                                       \
            static constexpr bool value = true; \
        };

// --------------------------------------------------------------------------------- //

/**
 * Set the size for the sequence.
 */
#define IMPL_MGL_DEFINE_SEQ_SIZE(NAME, ATTRIBUTES)  \
        template<>                                  \
        struct seq_size<NAME>                       \
        {                                           \
            static constexpr unsigned int value =   \
            IMPL_MGL_SEQ_SIZE(ATTRIBUTES);          \
        };

// --------------------------------------------------------------------------------- //

/**
 * @defgroup attributes Attributes module
 * @brief Everything related to the manipulation of the attributes (creation, use through shaders, ...)
 *
 * You'll find here the different classes that play an important role in the manipulation of attributes.\n
 * Basically, when using the part of this module, you will use the following:
 *  - #MGL_DEFINE_GL_ATTRIBUTES
 *      @code
 *          MGL_DEFINE_GL_ATTRIBUTES(
 *              (my_namespace),
 *              attributes,
 *              (glm::vec3, position)
 *              (glm::vec3, normal)
 *          )
 *      @endcode
 *  - at least one buffer defined like this:
 *      @code mgl::gl_vector<my_namespace::attributes> buffer; @endcode
 *  - gl_vao to automatically bind your buffer.
 * @{
 */

/**
 * @brief Macro to define attributes data.
 *
 * Usage :
 *  @code
 *      MGL_DEFINE_GL_ATTRIBUTES(
 *          (namespace0)(namespace1),
 *          attribute_block_name,
 *          (glm::vec3, attribute1)
 *          (int      , attribute2)
 *      )
 *  @endcode
 * Define attributes that can be drawn with a shader program.
 * The attributes name must be in the GLSL vertex shader
 * of the same name than in their declaration. For instance,
 * here the vertex shader would be :
 *  @code
 *      in vec3 attribute1;
 *      in int  attribute2;
 *
 *      ...
 *  @endcode
 * Notes :
 *  - You can't prefix an attribute with "gl_", if you do so the
 *    program will not link. No check is performed on it there, so
 *    it's your job to use it properly.
 *  - If you don't want to have your type in the global namespace do the following:
 *      @code
 *          MGL_DEFINE_GL_ATTRIBUTES(
 *              ,
 *              attributes,
 *              // same as above
 *          )
 *      @endcode
 */
#define MGL_DEFINE_GL_ATTRIBUTES(NAMESPACE_SEQ, NAME, ATTRIBUTES) \
        IMPL_MGL_NAMESPACE_START(NAMESPACE_SEQ) \
        struct NAME                             \
        {                                       \
            typedef NAME self_type;             \
                                                \
            IMPL_MGL_DEFINE_MEMBERS(ATTRIBUTES) \
        };                                      \
        IMPL_MGL_NAMESPACE_END(NAMESPACE_SEQ)   \
        namespace mgl {                         \
        IMPL_MGL_DEFINE_MEMBERS_NAMES(          \
            IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME, \
            ATTRIBUTES)                         \
        IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(   \
            IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME, \
            ATTRIBUTES)                         \
        IMPL_MGL_DEFINE_ATTRIBUTES_VALUE_AT(    \
            IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME, \
            ATTRIBUTES)                         \
        namespace priv {                        \
        IMPL_MGL_DEFINE_IS_GL_ATTRIBUTE(        \
            IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME) \
        IMPL_MGL_DEFINE_SEQ_SIZE(               \
            IMPL_MGL_EXPAND_NAMESPACE(NAMESPACE_SEQ) NAME, \
            IMPL_MGL_FILLER(ATTRIBUTES)) \
        }                                       \
        }

/**
 * @}
 */
//MGL_DEFINE_TEST(,test, (int, gg)(float, op))

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
//#define MGL_ADAPT_TO_GL_ATTRIBUTES(NAME, ATTRIBUTES)                                            \
//        BOOST_FUSION_ADAPT_STRUCT(NAME, ATTRIBUTES)                                             \
//        namespace mgl  {                                                                        \
//        namespace priv {                                                                        \
//            struct is_gl_attributes<NAME>                                                       \
//            {                                                                                   \
//                static constexpr bool value = true;                                             \
//            };                                                                                  \
//        }                                                                                       \
//        }                                                                                       \
//        IMPL_MGL_DEFINE_ATTRIBUTES_OFFSET_AT(                                                   \
//                NAME,                                                                           \
//                ATTRIBUTES)

#endif /* GLDATA_HPP_ */
