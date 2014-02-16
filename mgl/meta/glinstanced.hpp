/*
 * glinstanced.hpp
 *
 *  Created on: 16 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLINSTANCED_HPP_
#define GLINSTANCED_HPP_

#include <type_traits>

namespace mgl {

template<typename T, typename B>
class gl_vector;

template<typename T, typename B>
class gl_simple_buffer;

/**
 * \brief gl_instanced is the class turning on buffer data per instance
 */
template<typename T>
struct gl_instanced
{
    /**
     * \brief Change the use of this buffer when given to a vao.
     * \param p_buffer is the buffer that will be considered as an instanced data buffer.
     * \return Returns the wrapper.
     */
    template<typename U, typename V>
    friend gl_instanced<gl_vector<U, V>> make_instanced(const gl_vector<U, V>& p_buffer);
    template<typename U, typename V>
    friend gl_instanced<gl_simple_buffer<U, V>> make_instanced(const gl_simple_buffer<U, V>& p_buffer);

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Bind the instanced buffer.
     */
    inline void bind() const
    {
        m_buffer.bind();
    }

    /**
     * @brief Returns the underlying buffer.
     * @return Returns the underlying buffer.
     */
    inline const T & buffer() const
    {
        return m_buffer;
    }

    /**
     * \brief Returns the divisor number.
     * Currently returns just 1. That implies having
     * the buffer being modified every one instance.
     * \return Returns the divisor number
     */
    inline char get_divisor() const
    {
        return 1;
    }

private:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_instanced(const T& p_buffer)
        : m_buffer(p_buffer)
    {}

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The buffer that will behave like instanced data. */
    const T& m_buffer;
};

template<typename T, typename B>
typename std::enable_if<priv::is_gl_attributes<T>::value,
gl_instanced<gl_vector<T,B>>>::type make_instanced(const gl_vector<T, B>& p_buffer)
{
    return p_buffer;
}

template<typename T, typename B>
gl_instanced<gl_simple_buffer<T, B>> make_instanced(const gl_simple_buffer<T, B>& p_buffer)
{
    return p_buffer;
}

}  /* namespace mgl */


#endif /* GLINSTANCED_HPP_ */
