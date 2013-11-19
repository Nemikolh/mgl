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

namespace nkh {
namespace core {
namespace gl {

/**
 * \class gl_allocator is the allocator.
 */
template<typename T, typename Buff>
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

    template<typename U, typename B2 = Buff>
    struct rebind
    {
        typedef gl_allocator<U, B2> other;
    };

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default constructor.
     */
    gl_allocator() = default;

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
    template<typename T2, typename B2>
    gl_allocator(const gl_allocator<T2, B2>& p_rhs)
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

        // TODO : is it correct ? Doesn't it need an unmapping ? -> see it when doing the gl_vector.
//        if(p_cptr)
//            _ret(p_cptr);
//        else
            _ret.create();

        _ret.bind();
        gl_object_buffer<Buff>::gl_buffer_data(p_n * sizeof(T), nullptr);
        _ret.map_range(0, p_n);
        return _ret;
    }

    void deallocate(pointer p_ptr, size_type p_n)
    {
        // TODO should we unbind it before delete ?
        p_ptr.bind();
        p_ptr.unmap();
        gl_object_buffer<Buff>::gl_delete(1, &(p_ptr.m_id));
    }

    size_type max_size() const
    {
        return size_t(-1) / sizeof(T);
    }

};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template<typename T, typename B1, typename U, typename B2>
    inline bool operator==(gl_allocator<T, B1> a, gl_allocator<U, B2> b)
    {
        return true;
    }

    /**
     * operator !=
     */
    template<typename T, typename B1, typename U, typename B2>
    inline bool operator!=(gl_allocator<T, B1> a, gl_allocator<U, B2> b)
    {
        return false;
    }

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */

#endif /* GLALLOCATOR_HPP_ */
