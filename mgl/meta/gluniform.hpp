/*
 * gluniform.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLUNIFORM_HPP_
#define GLUNIFORM_HPP_

#include <glm/gtc/type_ptr.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include "../type/gltraits.hpp"

namespace mgl {

namespace priv {

struct uniform_helper
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    template<typename Uniform, typename Data>
    static inline
    typename std::enable_if<std::is_integral<typename Data::value_type>::value>::type
    glm_vector(const Uniform& p_uniform, Data p_valueToBind)
    {
        // TODO use metabind function to make the appropriate glUniform call.
        glcalli<tuple_size<Data>::value>::bindUniform(p_uniform.id(), p_valueToBind);
    }

    template<typename Uniform, typename Data>
    static inline
    typename std::enable_if<std::is_floating_point<typename Data::value_type>::value>::type
    glm_vector(const Uniform& p_uniform, Data p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniform(p_uniform.id(), p_valueToBind);
    }

    template<typename Uniform, typename Data>
    static inline void
    glm_matrix(const Uniform& p_uniform, Data p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniformMatrix(p_uniform.id(), p_valueToBind);
    }

private:
    // ================================================================ //
    // ============================ HELPERS =========================== //
    // ================================================================ //

    /* Implementation details. */
    template<unsigned int TupleSize>
    struct glcallf
    {
        template<typename Data>
        inline static void bindUniform(gl_types::id, Data p_value);
        template<typename Data>
        inline static void bindUniformMatrix(gl_types::id, Data p_value);
    };

    /* Implementation details. */
    template<unsigned int TupleSize>
    struct glcalli
    {
        template<typename Data>
        inline static void bindUniform(gl_types::id, Data p_value);
    };

#define MGL_DEF_UNIFORM_CALL(tuple_size, element_type, matrix)  \
    template<> \
    struct BOOST_PP_CAT(glcall,element_type)<tuple_size> \
    { \
        template<typename Data> \
        inline static void bindUniform(gl_types::id p_id, Data p_value)   \
        {   \
            BOOST_PP_CAT(glUniform,BOOST_PP_CAT(tuple_size,BOOST_PP_CAT(element_type,v)))(p_id, 1, glm::value_ptr(p_value)); \
        } \
        BOOST_PP_EXPR_IF(matrix,\
        template<typename Data> \
        inline static void bindUniformMatrix(gl_types::id p_id, Data p_value) \
        { \
            BOOST_PP_CAT(glUniformMatrix,BOOST_PP_CAT(tuple_size,BOOST_PP_CAT(element_type,v)))(p_id, 1, GL_FALSE, glm::value_ptr(p_value)); \
        } \
        ) \
    }

    /* Implementation details. */
    MGL_DEF_UNIFORM_CALL(4,f,1);
    MGL_DEF_UNIFORM_CALL(3,f,1);
    MGL_DEF_UNIFORM_CALL(2,f,1);

    /* Implementation details. */
    MGL_DEF_UNIFORM_CALL(4,i,0);
    MGL_DEF_UNIFORM_CALL(3,i,0);
    MGL_DEF_UNIFORM_CALL(2,i,0);
    MGL_DEF_UNIFORM_CALL(1,i,0);

#undef MGL_DEF_UNIFORM_CALL

};

}  /* namespace priv */
}  /* namespace mgl */

#endif /* GLUNIFORM_HPP_ */
