/*
 * glbindattrib.hpp
 *
 *  Created on: 19 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLBINDATTRIB_HPP_
#define GLBINDATTRIB_HPP_

#include "glbinder.hpp"
#include "../glmhelper.hpp"
#include "../meta/glutil.hpp"

namespace mgl {
namespace priv {

    /**
     * Meta function iterating over attributes of the Sequence.
     */
    template<typename Seq, typename AttributeBinder, typename N>
    struct bind_Iter
    {
        typedef typename value_at<Seq, N::value>::type     current_t;
        typedef struct_member_name<Seq, N::value>          name_t;

        constexpr static inline void map(const AttributeBinder & sh)
        {
            static_assert(tuple_size<current_t>::value < 5,"The tuple size must be either 1, 2, 3 or 4. GL_BGRA is not currently supported.");
            // ------------------------- DECLARE ------------------------ //

            sh(
                name_t::call(),                         // attribute name
                tuple_size<current_t>::value,           // number of component
                offset_at<Seq, N::value>::value,        // offsetof(Seq, name_t) conceptually
                sizeof(Seq),                            // stride
                tuple_component_type<current_t>::value  // deduce
            );

            bind_Iter<Seq, AttributeBinder, int_<N::value + 1>>::map(sh);
        }
    };

    /**
     * End of iteration.
     */
    template<typename Seq, typename AttributeBinder>
    struct bind_Iter<Seq, AttributeBinder, int_<seq_size<Seq>::value>>
    {
        constexpr static inline void map(...) {}
    };

    /**
     * Start of the iteration.
     */
    template<typename Seq, typename AttributeBinder>
    struct bind_first : bind_Iter<Seq, AttributeBinder, int_<0>> {};

    /**
     * External entry for the mapping.
     */
    template<typename Seq, typename AttributeBinder>
    struct bind_attributes
    {
        typedef bind_attributes<Seq, AttributeBinder> type;
        constexpr static inline void map(const AttributeBinder & sh)
        {
            bind_first<Seq, AttributeBinder>::map(sh);
        }
    };

} /* namespace priv */

/**
 * @ingroup attributes
 * @brief gl_bind_attributes is a class binding attributes automatically
 *        throught the use of the meta programming language.
 *
 * To use this functionnality, you must be using the macro defined
 * by the library :
 *      - #MGL_DEFINE_GL_ATTRIBUTES
 *
 *
 *  For the binding simply make the following call :
 *
 *      @code
 *          gl_bind_attributes< T >::map(attribute_bind)
 *      @endcode
 *
 *  where T is your attribute structure, and attribute_bind is
 *  a functor defined as follow :
 *
 *      @code
 *          void operator()(
 *              char const*,                        // attribute name
 *              int,                                // number of component
 *              std::size_t,                        // offsetof(Seq, name_t)
 *              std::size_t,                        // stride
 *              GLenum                              // type of the component (GL_FLOAT, ...)
 *          );
 *      @endcode
 *
 *
 * @param T is the structure defined with NKH_DEFINE_GLATTRIBUTES
 */
template<typename T>
struct gl_bind_attributes : public priv::bind_attributes<T, gl_attribute_binder>::type
{};


} /* namespace mgl */

#endif /* GLBINDATTRIB_HPP_ */
