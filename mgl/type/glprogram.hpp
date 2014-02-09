/*
 * glprogram.hpp
 *
 *  Created on: 21 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLPROGRAM_HPP_
#define GLPROGRAM_HPP_

#include <utility>
#include <type_traits>
#include "glvao.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

namespace nkh {
namespace core {
namespace gl {

/**
 * \class gl_program is a wrapper for an opengl shader program.
 */
struct gl_program
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //


    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief This method allows to create a vao based on this program.
     * \param p_buffers is the buffers that are gonna be part of this vao.
     * \return Returns the created vao.
     */
    template<typename... T>
    gl_vao make_vao(T&&... p_buffers)
    {
        ensure_linked();
        return gl_vao(id(), std::forward<T>(p_buffers)...);
    }

    /**
     * \brief Returns the id of this program.
     * \return Returns the opengl id.
     */
    gl_types::id id()
    {
        return 0;
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform{1[2|3|4}{i|f}v for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mlg::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<is_vector<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        setVectorImpl(p_uniform, std::move(p_valueToBind));
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniformMatrix{2|3|4}fv for glm types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mlg::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<is_matrix<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        setMatrixImpl(p_uniform, std::move(p_valueToBind));
    }

    /**
     * \brief Set the passed value to the passed uniform.
     * This is the equivalent to the call to glUniform{i|f} for arithmetic types.
     * This is done through traits. So if you want to use it for your own vector type, you
     * have two options. First you may add the following nested typedef :
     *  \code
     *      typedef \/\* your component type \*\/value_type;
     *  \endcode
     * This will compute the value as a simple division. The second solution is specializing
     * the mlg::tuple_size meta function for your type.
     * \param p_uniform is the uniform element to set.
     * \param p_valueToBind is the value desired.
     */
    template<typename Uniform, typename Data>
    typename std::enable_if<std::is_arithmetic<Data>::value>::type
    set(const Uniform& p_uniform, Data p_valueToBind)
    {
        // TODO
    }

private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Perform the linking of the program if note already done.
     */
    void ensure_linked()
    {

    }

    template<typename Uniform, typename Data>
    typename std::enable_if<std::is_integral<typename Data::value_type>::value>::type
    setVectorImpl(const Uniform& p_uniform, Data&& p_valueToBind)
    {
        // TODO use metabind function to make the appropriate glUniform call.
        glcalli<tuple_size<Data>::value>::bindUniform(p_uniform.id(), std::move(p_valueToBind));
    }

    template<typename Uniform, typename Data>
    typename std::enable_if<std::is_floating_point<typename Data::value_type>::value>::type
    setVectorImpl(const Uniform& p_uniform, Data&& p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniform(p_uniform.id(), std::move(p_valueToBind));
    }

    template<typename Uniform, typename Data>
    void
    setMatrixImpl(const Uniform& p_uniform, Data&& p_valueToBind)
    {
        glcallf<tuple_size<Data>::value>::bindUniformMatrix(p_uniform.id(), std::move(p_valueToBind));
    }

    // ================================================================ //
    // ============================ HELPERS =========================== //
    // ================================================================ //

    /* Implementation details. */
    template<unsigned int TupleSize>
    struct glcallf
    {
        template<typename Data>
        inline static void bindUniform(gl_types::id, Data&& p_value);
        template<typename Data>
        inline static void bindUniformMatrix(gl_types::id, Data&& p_value);
    };

    /* Implementation details. */
    template<unsigned int TupleSize>
    struct glcalli
    {
        template<typename Data>
        inline static void bindUniform(gl_types::id, Data&& p_value);
    };

#define NKH_GL_DEF_UNIFORM_CALL(tuple_size, element_type, matrix)  \
    template<> \
    struct BOOST_PP_CAT(glcall,element_type)<tuple_size> \
    { \
        template<typename Data> \
        inline static void bindUniform(gl_types::id p_id, Data&& p_value)   \
        {   \
            BOOST_PP_CAT(glUniform,BOOST_PP_CAT(tuple_size,BOOST_PP_CAT(element_type,v)))(p_id, 1, glm::value_ptr(p_value)); \
        } \
        BOOST_PP_EXPR_IF(matrix,\
        template<typename Data> \
        inline static void bindUniformMatrix(gl_types::id p_id, Data&& p_value) \
        { \
            BOOST_PP_CAT(glUniformMatrix,BOOST_PP_CAT(tuple_size,BOOST_PP_CAT(element_type,v)))(p_id, 1, GL_FALSE, glm::value_ptr(p_value)); \
        } \
        ) \
    }

    /* Implementation details. */
    NKH_GL_DEF_UNIFORM_CALL(4,f,1);
    NKH_GL_DEF_UNIFORM_CALL(3,f,1);
    NKH_GL_DEF_UNIFORM_CALL(2,f,1);

    /* Implementation details. */
    NKH_GL_DEF_UNIFORM_CALL(4,i,0);
    NKH_GL_DEF_UNIFORM_CALL(3,i,0);
    NKH_GL_DEF_UNIFORM_CALL(2,i,0);
    NKH_GL_DEF_UNIFORM_CALL(1,i,0);

#undef NKH_GL_DEF_UNIFORM_CALL
};


} /* namespace gl. */
} /* namespace core. */
} /* namespace nkh. */


#endif /* GLPROGRAM_HPP_ */
