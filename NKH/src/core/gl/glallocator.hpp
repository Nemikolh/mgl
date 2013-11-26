/*
 * glallocator.hpp
 *
 *  Created on: 24 juin 2013
 *      Author: nemikolh
 */

#ifndef GLALLOCATOR_HPP_
#define GLALLOCATOR_HPP_

#include <type_traits>
#include <memory>
#include "glptr.hpp"

namespace mgl {

/**
 * \class gl_allocator is the allocator.
 */
template<typename T, typename Container, typename Buff>
class gl_allocator
{
public:
    // ================================================================ //
    // ========================= STATIC ASSERT ======================== //
    // ================================================================ //

    static_assert(std::is_standard_layout<T>::value, "The type used here must be a standard layout data type.");

    // ================================================================ //
    // ============================ TYPEDEF =========================== //
    // ================================================================ //

    /**
     * Allocator requirements.
     */
    typedef gl_ptr<T, Buff>             pointer;
    typedef const gl_ptr<T, Buff>       const_pointer;
    typedef gl_ptr<void, Buff>          void_pointer;
    typedef const gl_ptr<void, Buff>    const_void_pointer;


    typedef T                               value_type;
    typedef size_t                          size_type;
    typedef typename pointer::difference_type
                                            difference_type;

    template<typename T2, typename C2 = Container, typename B2 = Buff>
    struct rebind
    {
        typedef gl_allocator<T2, C2, B2> other;
    };

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default constructor.
     */
    explicit gl_allocator(const Container& p_owner)
        : m_owner(p_owner)
    {}

    /**
     * \brief Default copy constructor.
     */
    gl_allocator(const gl_allocator& ) = default;

    /**
     * \brief Default move constructor.
     */
    gl_allocator(gl_allocator&&) = default;

    /**
     * \brief Copy constructor
     * \param p_rhs is the allocator we copy from.
     */
    template<typename T2, typename C2, typename B2>
    gl_allocator(const gl_allocator<T2, C2, B2>& p_rhs)
        : m_owner(p_rhs.m_owner)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    pointer allocate(size_type p_n, const_pointer p_cptr = nullptr)
    {
        pointer _ret = nullptr;
#ifndef NKH_NDEBUG
        // TODO : context.
        //if(!_ret.hasValidContext())
        //    throw gl_context_exception();
#endif
        if (p_n > this->max_size())
            throw std::bad_alloc();

        if(m_owner.m_id == 0)
        {
            gl_object_buffer<Buff>::gl_gen(1, &(m_owner.m_id));
            gl_object_buffer<Buff>::gl_bind(m_owner.m_id);
            gl_object_buffer<Buff>::gl_buffer_data(p_n * sizeof(T), nullptr);
        }
        _ret.map_range(0, p_n);
        return _ret;
    }

    void deallocate(pointer p_ptr, size_type p_n)
    {
        p_ptr.unmap();
        gl_object_buffer<Buff>::gl_delete(1, &(m_owner.m_id));
        m_owner.m_id = 0;
    }

    size_type max_size() const
    {
        return size_t(-1) / sizeof(T);
    }

private:

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The owner of this instance of allocator. */
    const Container& m_owner;
};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template<typename T, typename C1, typename B1, typename U, typename C2, typename B2>
    inline bool operator==(gl_allocator<T, C1, B1> a, gl_allocator<U, C2, B2> b)
    {
        return true;
    }

    /**
     * operator !=
     */
    template<typename T, typename C1, typename B1, typename U, typename C2, typename B2>
    inline bool operator!=(gl_allocator<T, C1, B1> a, gl_allocator<U, C2, B2> b)
    {
        return false;
    }

} /* namespace mgl */

#endif /* GLALLOCATOR_HPP_ */
