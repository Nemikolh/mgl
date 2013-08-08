/*
 * glscope.hpp
 *
 *  Created on: 8 août 2013
 *      Author: nemikolh
 */

#ifndef GLSCOPE_HPP_
#define GLSCOPE_HPP_


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

    gl_scope(T & t);

    ~gl_scope()
    {
    }
};

template<typename T, typename Buff, typename Alloc>
class gl_scope< gl_vector<T, Buff, Alloc> >
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_scope(gl_vector<T, Buff, Alloc> & p_vector)
        : m_saver{p_vector.begin()}
    {
        m_saver.base().bind();
        m_saver.base().map_range(0, p_vector.capacity());
    }

    ~gl_scope()
    {
        m_saver.base().bind();
        m_saver.base().unmap();
    }

private:

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    typename gl_vector<T, Buff, Alloc>::iterator m_saver;
};

} /* namepsace gl. */
} /* namespace core. */
} /* namespace nkh. */

#endif /* GLSCOPE_HPP_ */
