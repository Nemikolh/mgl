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
    template<typename U>
    static typename U::col_type   deduceColType(U*);
    static T                      deduceColType(...);

    template<typename U>
    static typename U::row_type   deduceRowType(U*);
    static T                      deduceRowType(...);
public:
    typedef is_matrix<T> type;
    typedef bool         value_type;

    static constexpr bool value =
#if GLM_VERSION == 94
            glm::detail::is_matrix<T>::_YES == 1;
#elif GLM_VERSION == 95
    sizeof(decltype(deduceColType((T*)0))) == 1 && sizeof(decltype(deduceRowType((T*)0))) == 1;
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
            !is_matrix<T>::value && T::length() <= 4;
#endif
};

}

#endif /* MGL_META_GLMHELPER_HPP_ */
