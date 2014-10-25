/*
 * gluniformhelpers.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLUNIFORMHELPERS_HPP_
#define GLUNIFORMHELPERS_HPP_

// TODO: move this header out of here.
//       create a meta function encapsulating glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
#include "../type/gltraits.hpp"
#include "../preprocessor/glpreprocessor_base.hpp"

namespace mgl {

namespace priv {

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
struct IMPL_MGL_CAT(glcall,element_type)<tuple_size> \
{ \
    template<typename Data> \
    inline static void bindUniform(gl_types::id p_id, Data p_value)   \
    {   \
        IMPL_MGL_CAT(glUniform,IMPL_MGL_CAT(tuple_size,IMPL_MGL_CAT(element_type,v)))(p_id, 1, glm::value_ptr(p_value)); \
    } \
    IMPL_MGL_IF(matrix,\
    template<typename Data> \
    inline static void bindUniformMatrix(gl_types::id p_id, Data p_value) \
    { \
        IMPL_MGL_CAT(glUniformMatrix,IMPL_MGL_CAT(tuple_size,IMPL_MGL_CAT(element_type,v)))(p_id, 1, GL_FALSE, glm::value_ptr(p_value)); \
    } \
    ,) \
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

struct uniform_helper
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    template<typename Uniform, typename Data>
    static inline
    typename std::enable_if<std::is_integral<typename Data::value_type>::value>::type
    glm_vector(Uniform p_uniform, Data p_valueToBind)
    {
        // TODO use metabind function to make the appropriate glUniform call.
        glcalli<tuple_size<Data>::value>::bindUniform(p_uniform.id(), p_valueToBind);
    }

    template<typename Uniform, typename Data>
    static inline
    typename std::enable_if<std::is_floating_point<typename Data::value_type>::value>::type
    glm_vector(Uniform p_uniform, Data p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniform(p_uniform.id(), p_valueToBind);
    }

    template<typename Uniform, typename Data>
    static inline void
    glm_matrix(Uniform p_uniform, const Data& p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniformMatrix(p_uniform.id(), p_valueToBind);
    }
};

}  /* namespace priv */
}  /* namespace mgl */

#endif /* GLUNIFORMHELPERS_HPP_ */
