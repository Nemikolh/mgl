/*
 * glsinglebuffer.hpp
 *
 *  Created on: 16 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLSINGLEBUFFER_HPP_
#define GLSINGLEBUFFER_HPP_

#include <type_traits>
#include "../glfwd.hpp"

namespace mgl {

namespace priv {

template<typename T>
struct is_gl_attributes;

}  /* namespace priv */

/**
 * @ingroup attributes
 * @brief The gl_simple_buffer type is a helper for gl_vector<T> where T is a primitive type.
 *
 * When you use gl_vector with a type which isn't defines with #MGL_DEFINE_GL_ATTRIBUTES like int or float,
 * you end up with a problem when you want to bind it to a vao. \n
 * The problem is, as you didn't provide a name to the attribute, how does the vao is supposed to
 * handle your buffer? To which attribute should be bound the buffer? \n
 *
 * The answer is this class. You need this class to tell the vao how to manage your buffer.
 *
 * Note:
 *  - This class isn't intended to be used directly. Use mgl::make_buffer function when you
 *    need the behavior provided by this class.
 *  - As the class hold a const reference on your buffer. If you don't use it properly, you
 *    may get crashes linked to the dangling reference when you outlives the wrapped gl_vector object.
 */
template<typename T, typename B>
struct gl_simple_buffer
{
    template<typename U, typename V>
    friend gl_simple_buffer<U, V> make_buffer(const gl_vector<U, V>&, const char*);

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Bind the instanced buffer.
     */
    inline void bind() const
    {
        m_buffer.bind();
    }


    /**
     * @brief Returns the chosen attribute name.
     * @return Return the attribute's name.
     */
    inline const char* attribute_name() const
    {
        return m_name;
    }

    /**
     * @brief Returns the underlying buffer.
     * @return Returns the underlying buffer.
     */
    inline const gl_vector<T, B> & buffer() const
    {
        return m_buffer;
    }

private:

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_simple_buffer(const gl_vector<T,B> & p_buffer, const char* p_name)
        : m_buffer(p_buffer)
        , m_name(p_name)
    {}

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    const gl_vector<T, B> & m_buffer;
    const char* m_name;
};



/**
 * @ingroup attributes
 * @brief Enables to bind automatically a buffer which does not have a name.
 * This function give a name to the passed buffer.
 * Note that you can't call the function on a buffer defined by #MGL_DEFINE_GL_ATTRIBUTES.
 * @param p_buffer is the name of the buffer.
 * @param p_name is the name for the attribute.
 * @return Returns a clue for the automatic binding buffers facility.
 */
template<typename T, typename B>
gl_simple_buffer<T, B> make_buffer(const gl_vector<T, B>& p_buffer, const char* p_name)
{
    static_assert(!priv::is_gl_attributes<T>::value, "T must be a single attribute data type (glm::vec3, float, ...)");
    return gl_simple_buffer<T, B>(p_buffer, p_name);
}


}  /* namespace mgl */


#endif /* GLSINGLEBUFFER_HPP_ */
