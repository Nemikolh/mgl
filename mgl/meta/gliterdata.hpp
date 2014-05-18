/*
 * gliterdata.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef GLITERDATA_HPP_
#define GLITERDATA_HPP_

#include "glutil.hpp"

namespace mgl {
namespace priv {

    /**
     * Meta function iterating over attributes of the Sequence.
     */
    template<typename Seq, typename ApplyFunctor, typename N>
    struct iter_base
    {
        typedef typename value_at<Seq, N::value>::type     current_t;
        typedef struct_member_name<Seq, N::value>          name_t;

        static inline void apply(ApplyFunctor&& p_func)
        {
            p_func.template apply<current_t, N::value>(name_t::call());
            iter_base<Seq, ApplyFunctor, int_<N::value + 1>>::apply(p_func);
        }
    };

    /**
     * End of iteration.
     */
    template<typename Seq, typename ApplyFunctor>
    struct iter_base<Seq, ApplyFunctor, int_<seq_size<Seq>::value>>
    {
        static inline void apply(ApplyFunctor&&) {}
    };

    /**
     * Start of the iteration.
     */
    template<typename Seq, typename ApplyFunctor>
    struct iter_first : iter_base<Seq, ApplyFunctor, int_<0>> {};


} /* namespace priv */

template<typename T>
struct for_each
{
    template<typename ApplyFunctor>
    static inline void apply(ApplyFunctor&& p_func)
    {
        priv::iter_first<T, ApplyFunctor>::apply(p_func);
    }
};


} /* namespace mgl */

#endif /* GLITERDATA_HPP_ */
