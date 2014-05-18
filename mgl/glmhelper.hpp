/*
 * glmhelper.hpp
 *
 *  Created on: 30 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLMHELPER_HPP_
#define GLMHELPER_HPP_

#include <type_traits>
// TODO : may have change with the last release of mgl.
#include <glm/core/_detail.hpp>

#include "meta/glutil.hpp"

namespace mgl {

/**
 * \class is_vector is the std conformant meta function base on glm::detail::is_vector.
 */
template<typename T>
struct is_vector
{
    typedef is_vector<T> type;
    typedef bool         value_type;

    static constexpr bool value = glm::detail::is_vector<T>::_YES == 1;
};

/**
 * \class is_matrix is the std conformant meta function based on glm::detail::is_matrix
 */
template<typename T>
struct is_matrix
{
    typedef is_matrix<T> type;
    typedef bool         value_type;
    static constexpr bool value = glm::detail::is_matrix<T>::_YES == 1;
};



namespace priv {

/**
 * \class tuple_size_glm compute the size of a glm type when it is seen as a tuple.
 *
 *  glm types are not at as defined in the standard "tuples". But as here,
 * we do need the number of component of an attribute, this is a convenient way to do it.
 *
 * Implementation note :
 * ---------------------
 *
 *  The code is using the inner presence of a nested value_type inside glm types.
 *  Do not make assumption on it, as it can change in the same way the glm is getting
 *  updated.
 *
 */
template<typename T>
class tuple_size_glm
{
    template<typename U>
    static typename U::value_type   deduce(U*);
    static T                        deduce(...);
public:

    typedef
        typename divides<
                    int_<sizeof(T)>
                ,   int_<sizeof(decltype(deduce((T*)0)))>
            >::type type;
    static constexpr typename type::value_type
                    value = type::value;
};

}  /* namespace priv */



/**
 * Meta function returning one if the type is arithmetic, otherwise
 * try to count the size of the "tuple".
 */
template<typename T>
struct tuple_size
{
    typedef
        typename priv::eval_if< std::is_arithmetic<T>
            , priv::int_<1>
            , priv::tuple_size_glm<T>
        >::type         type;
    static constexpr typename type::value_type value = type::value;
};



}  /* namespace mgl */


#endif /* GLMHELPER_HPP_ */
