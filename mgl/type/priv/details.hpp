/*
 * details.hpp
 *
 *  Created on: 6 avr. 2014
 *      Author: nemikolh
 */

#ifndef TYPE_PRIV_DETAILS_HPP_
#define TYPE_PRIV_DETAILS_HPP_

#include "../gltraits.hpp"

namespace mgl {
namespace priv {

struct base_id_ref_count
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    base_id_ref_count() noexcept
        : m_id(0)
        , m_ref_count(nullptr)
    {}

    base_id_ref_count(const base_id_ref_count& p_rhs)
        : m_id(p_rhs.m_id)
        , m_ref_count(p_rhs.m_ref_count)
    {
#       ifndef MGL_NDEBUG
        assert(m_ref_count);
#       endif
        incr_ref_count();
    }

    base_id_ref_count& operator=(const base_id_ref_count& p_rhs)
    {
        if(p_rhs.m_ref_count != m_ref_count)
        {
#           ifndef MGL_NDEBUG
            assert(m_id != p_rhs.m_id);
#           endif
            release();
            incr_ref_count();
            m_id = p_rhs.m_id;
            m_ref_count = p_rhs.m_ref_count;
        }
        return *this;
    }

    base_id_ref_count(base_id_ref_count&& p_rhs)
        : m_id(p_rhs.m_id)
        , m_ref_count(p_rhs.m_ref_count)
    {
        p_rhs.m_id = 0;
        p_rhs.m_ref_count = nullptr;
    }

    base_id_ref_count& operator=(base_id_ref_count&& p_rhs)
    {
        // Seems a bit useless but needed to prevent from cast with std::move().
        if(p_rhs.m_ref_count != m_ref_count)
        {
#           ifndef MGL_NDEBUG
            assert(m_id != p_rhs.m_id);
#           endif
            release();
            m_id = p_rhs.m_id;
            m_ref_count = p_rhs.m_ref_count;
            p_rhs.m_id = 0;
            p_rhs.m_ref_count = nullptr;
        }
        return *this;
    }

    virtual ~base_id_ref_count()
    {
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief The gl_shader can be converted to bool and is true if it does correspond to a real shader.
     */
    explicit operator bool() const
    {
        return m_ref_count && m_id;
    }

    /**
     * @brief Reset this object to point to a non-valid shader.
     */
    void release()
    {
        if(m_id && (*m_ref_count) == 1)
        {
            this->gl_delete(m_id);
            delete m_ref_count;
            m_ref_count = nullptr;
        }
        decr_ref_count();
        m_id = 0;
        m_ref_count = nullptr;
    }

    /**
     * @brief Returns the object's id.
     * @return Returns the object's id.
     */
    gl_types::id id() const
    {
        return m_id;
    }

protected:
    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    void ensure_created()
    {
        if(!m_id)
        {
            m_id = this->gen();
            m_ref_count = new unsigned int;
            *m_ref_count = 1;
        }
    }

    virtual gl_types::id gen() = 0;
    virtual void gl_delete(gl_types::id p_id) = 0;

private:
    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    inline void incr_ref_count()
    {
        if(m_ref_count)
            ++(*m_ref_count);
    }

    inline void decr_ref_count()
    {
        if(m_ref_count)
            --(*m_ref_count);
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The id of the openGL object. */
    gl_types::id m_id;
    /** Refcount on this shader. */
    unsigned int* m_ref_count;
};

}  /* namespace priv */
}  /* namespace mgl */


#endif /* TYPE_PRIV_DETAILS_HPP_ */
