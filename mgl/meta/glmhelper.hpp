/*
 * glmhelper.hpp
 *
 *  Created on: 18 oct. 2014
 *      Author: nemikolh
 */

#ifndef MGL_META_GLMHELPER_HPP_
#define MGL_META_GLMHELPER_HPP_

#ifdef GLM_VERSION
#   if GLM_VERSION == 94
#       include <glm/core/_detail.hpp>
#   elif GLM_VERSION == 95
#       include <glm/vec3.hpp>
#       include <glm/vec2.hpp>
#       include <glm/vec4.hpp>
#   else
#       warning "GLM version not supported. You might end with weird behavior."
#   endif
#else
// Move this within a Cmake configuration file.
#warning "GLM is not included."
#endif

namespace mgl {

/**
 * @brief is_matrix is the std conformant meta function to know if a type is a matrix type or not.
 */
template<typename T>
class is_matrix
{
    struct true_type    { char a[1]; };
    struct false_type   { char a[2]; };

    template<typename U>
    static true_type    deduceColType(U*, typename U::col_type* = 0);
    static false_type   deduceColType(...);

    template<typename U>
    static true_type    deduceRowType(U*, typename U::row_type* = 0);
    static false_type   deduceRowType(...);
public:
    typedef is_matrix<T> type;
    typedef bool         value_type;

    static constexpr bool value =
#if GLM_VERSION == 94
            glm::detail::is_matrix<T>::_YES == 1;
#elif GLM_VERSION == 95
    sizeof(decltype(deduceColType((T*)0))) == sizeof(true_type) &&
    sizeof(decltype(deduceRowType((T*)0))) == sizeof(true_type);
#endif
};

/**
 * @brief is_vector is the std conformant meta function returning true if T seems to be a vector type.
 */
template<typename T>
struct is_vector
{
    typedef is_vector<T> type;
    typedef bool         value_type;

    static constexpr bool value =
#if GLM_VERSION == 94
            glm::detail::is_vector<T>::_YES == 1;
#elif GLM_VERSION == 95
            false;
#endif
};

#if GLM_VERSION >= 95
#define SET_IS_VECTOR_TRUE(name)        \
    template<>                          \
    struct is_vector<glm::name>         \
    {                                   \
        typedef bool      value_type;   \
        static constexpr bool value =   \
                        true;           \
    }

SET_IS_VECTOR_TRUE(vec2);
SET_IS_VECTOR_TRUE(vec3);
SET_IS_VECTOR_TRUE(vec4);

SET_IS_VECTOR_TRUE(uvec2);
SET_IS_VECTOR_TRUE(uvec3);
SET_IS_VECTOR_TRUE(uvec4);

SET_IS_VECTOR_TRUE(ivec2);
SET_IS_VECTOR_TRUE(ivec3);
SET_IS_VECTOR_TRUE(ivec4);

SET_IS_VECTOR_TRUE(bvec2);
SET_IS_VECTOR_TRUE(bvec3);
SET_IS_VECTOR_TRUE(bvec4);

#undef SET_IS_VECTOR_TRUE
#endif
}

#endif /* MGL_META_GLMHELPER_HPP_ */
