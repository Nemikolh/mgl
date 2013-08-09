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
 * \brief gl_scope is a class that preserve a gl_state for a scope.
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
    gl_scope(T & t)
        : m_saver{t}
    {
        gl_state_traits<T>::save_state();
        m_saver.bind();
    }

    /**
     * \brief Destructor.
     */
    ~gl_scope()
    {
        m_saver.unbind();
        gl_state_traits<T>::restore_state();
    }

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The state bound and unbound during the lifetime of this object. */
    T & m_saver;
};

} /* namepsace gl. */
} /* namespace core. */
} /* namespace nkh. */

#endif /* GLSCOPE_HPP_ */
