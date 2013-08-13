/*
 * glmetabind.hpp
 *
 *  Created on: 19 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLMETABIND_HPP_
#define GLMETABIND_HPP_

#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/sizeof.hpp>

#include "../glrequires.hpp"
#include "../glshadercontext.hpp"
#include "gltraits.hpp"

namespace nkh {
namespace core {
namespace gl {

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
            typename boost::mpl::divides<
                        boost::mpl::sizeof_<T>
                    ,   boost::mpl::sizeof_<decltype(deduce((T*)0))>
                >::type type;
        static constexpr typename type::value_type
                        value = type::value;
    };

    /**
     * Meta function returning one if the type is arithmetic, otherwise
     * try to count the size of the "tuple".
     */
    template<typename T>
    struct tuple_size
    {
        typedef
            typename boost::mpl::eval_if< std::is_arithmetic<T>
                , boost::mpl::int_<1>
                , tuple_size_glm<T>
            >::type         type;
        static constexpr typename type::value_type value = type::value;
    };


    /**
     * Meta function iterating over attributes of the Sequence.
     */
    template<typename Seq, typename ShaderContext, typename N>
    struct bind_Iter
    {
        typedef typename boost::fusion::result_of::value_at<Seq, N>::type   current_t;
        typedef typename boost::mpl::next<N>::type                          next_t;
        typedef boost::fusion::extension::struct_member_name<Seq, N::value> name_t;

        static inline void map(const ShaderContext & sh, std::size_t byte_offset)
        {
            static_assert(tuple_size<current_t>::value < 5,"The tuple size must be either 1, 2, 3 or 4. GL_BGRA is not currently supported.");
            // ------------------------- DECLARE ------------------------ //

            sh.bind_attribute(
                name_t::call(),                         // attribute name
                tuple_size<current_t>::value,           // number of component
                byte_offset,                            // offsetof(Seq, name_t)
                sizeof(Seq),                            // stride
                tuple_component_type<current_t>::value  // deduce
            );

            bind_Iter<Seq, ShaderContext, next_t>::map(sh, byte_offset + sizeof(current_t));
        }
    };

    /**
     * End of iteration.
     */
    template<typename Seq, typename ShaderContext>
    struct bind_Iter<Seq, ShaderContext, typename boost::fusion::result_of::size<Seq>::type>
    {
        static inline void map(...) {}
    };

    /**
     * Start of the iteration.
     */
    template<typename Seq, typename ShaderContext>
    struct bind_first : bind_Iter<Seq, ShaderContext, boost::mpl::int_<0> > {};

    /**
     * External entry for the mapping.
     */
    template<typename Seq, typename ShaderContext>
    struct bind_attributes
    {
        typedef bind_attributes<Seq, ShaderContext> type;
        static inline void map(const ShaderContext & sh)
        {
            bind_first<Seq, ShaderContext>::map(sh, 0);
        }
    };

} /* namespace priv */

/**
 * \class gl_bind_attributes is a class binding attributes automatically
 *        throught the use of the meta programming language.
 * To use this functionnality, you must be using the macro defined
 * by the library :
 *      - NKH_DEFINE_GLATTRIBUTES
 *
 *
 *  For the binding simply make the following call :
 *
 *      \code
 *          gl_bind_attributes< T >::map(shader_context)
 *      \endcode
 *
 *  where T is your attribute structure, and shader_context is
 *  a type with a public member function defined as following :
 *
 *      \code
 *          void bind_attribute(
 *              char const*,                        // attribute name
 *              int,                                // number of component
 *              std::size_t,                        // offsetof(Seq, name_t)
 *              std::size_t,                        // stride
 *              GLenum                              // type of the component (GL_FLOAT, ...)
 *          );
 *      \endcode
 *
 *
 * \param T is the structure defined with NKH_DEFINE_GLATTRIBUTES
 * \param M is the material defined to be used in the shader_context.
 */
template<typename T, typename M>
struct gl_bind_attributes : public priv::bind_attributes<T, gl_shader_context<M> >::type
{};

/**
 * ------------------------------------------------------------------------
 * Basic Usage :
 * -------------
 *
 *      int main()
 *      {
 *          gl_shader_context<int> sh;
 *          gl_bind_attributes<nkh::core::data::mesh::vertex, int>::map(sh);
 *      }
 */

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */


#endif /* GLMETABIND_HPP_ */
