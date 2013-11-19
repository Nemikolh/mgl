/*
 * glscope.hpp
 *
 *  Created on: 8 août 2013
 *      Author: nemikolh
 */

#ifndef GLSCOPE_HPP_
#define GLSCOPE_HPP_

#include "meta/gltraits.hpp"

namespace nkh {
namespace core {
namespace gl {

/**
 * \brief gl_scope is a class that preserve a gl_state for a scope, and automatically bind the object.
 */
template<typename T>
class gl_scope
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default constructor.
     * \param The passed parameter is locked.
     */
    gl_scope(const T & t)
        : m_obj(t)
    {
        gl_state_traits<T>::save_state();
        m_obj.bind();
    }

    /**
     * \brief Destructor.
     */
    ~gl_scope()
    {
        m_obj.unbind();
        gl_state_traits<T>::restore_state();
    }

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The state bound and unbound during the lifetime of this object. */
    const T & m_obj;
};

template<typename T>
gl_scope<T> bind_at_scope(const T& p_obj)
{
    return gl_scope<T>(p_obj);
}

} /* namepsace gl. */
} /* namespace core. */
} /* namespace nkh. */

#endif /* GLSCOPE_HPP_ */
