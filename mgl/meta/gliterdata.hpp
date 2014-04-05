/*
 * gliterdata.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef GLITERDATA_HPP_
#define GLITERDATA_HPP_

//#include <boost/fusion/adapted/struct/define_struct.hpp>
//#include <boost/fusion/include/sequence.hpp>
//#include <boost/fusion/include/algorithm.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/include/is_sequence.hpp>
//#include <boost/mpl/eval_if.hpp>
//#include <boost/type_traits.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/sizeof.hpp>


namespace mgl {
namespace priv {

    /**
     * Meta function iterating over attributes of the Sequence.
     */
    template<typename Seq, typename ApplyFunctor, typename N>
    struct iter_base
    {
        typedef typename boost::fusion::result_of::value_at<Seq, N>::type   current_t;
        typedef typename boost::mpl::next<N>::type                          next_t;
        typedef boost::fusion::extension::struct_member_name<Seq, N::value> name_t;

        constexpr static inline void apply(const ApplyFunctor& p_func)
        {
            p_func.apply<current_t>(name_t::call(), N::value);
        }
    };

    /**
     * End of iteration.
     */
    template<typename Seq, typename ApplyFunctor>
    struct iter_base<Seq, ApplyFunctor, typename boost::fusion::result_of::size<Seq>::type>
    {
        constexpr static inline void apply(...) {}
    };

    /**
     * Start of the iteration.
     */
    template<typename Seq, typename ApplyFunctor>
    struct iter_first : iter_base<Seq, ApplyFunctor, boost::mpl::int_<0> > {};


} /* namespace priv */

template<typename T, typename ApplyFunctor>
struct for_each
{
    static inline void apply(const ApplyFunctor& p_func)
    {
        priv::iter_first<T, ApplyFunctor>::apply(p_func);
    }
};


} /* namespace mgl */

#endif /* GLITERDATA_HPP_ */
