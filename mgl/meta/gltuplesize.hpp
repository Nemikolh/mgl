/*
 * gltuplesize.hpp
 *
 *  Created on: 30 sept. 2013
 *      Author: nemikolh
 */

#ifndef MGL_GLTUPLESIZE_HPP_
#define MGL_GLTUPLESIZE_HPP_

#include <type_traits>
#include "glutil.hpp"

namespace mgl {
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


#endif /* MGL_GLTUPLESIZE_HPP_ */
