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

namespace nkh {
namespace core {
namespace gl {

/**
 * \class gl_allocator is the allocator.
 */
template<typename T, typename Buff = gl_buffer<T> >
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
     * Allocator traits definition.
     */
    typedef gl_ptr<T, Buff>                         pointer;
    typedef const gl_ptr<T, Buff>                   const_pointer;
    typedef typename pointer::void_pointer          void_pointer;
    typedef typename pointer::const_void_pointer    const_void_pointer;


    typedef T                               value_type;
    typedef size_t                          size_type;
    typedef typename pointer::ptrdiff_t     difference_type;

    template<typename U>
    struct rebind
    {
        typedef gl_allocator<U> other;
    };

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    pointer allocate(size_type p_n, const void * = 0)
    {
        pointer _ret = nullptr;
#ifndef NKH_NDEBUG
        if(!_ret.hasValidContext())
            throw gl_context_exception();
#endif
        if (p_n > this->max_size())
            throw std::bad_alloc();

        _ret.create();
        _ret.bind();
        glCheck(glBufferData(Buff::target, p_n * sizeof(T), Buff::usage));
        _ret.unbind();
        return _ret;
    }

    void deallocate(pointer p_ptr, size_type p_n)
    {
        glCheck(glDeleteBuffers(1, &p_ptr.m_id));
    }

    size_type max_size() const
    {
        return size_t(-1) / sizeof(T);
    }

    cf http://en.cppreference.com/w/cpp/concept/Allocator pour la suite.
};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template<typename T, typename U>
    inline bool operator==(gl_allocator<T> a, gl_allocator<U> b)
    {
        return true;
    }

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */

#endif /* GLALLOCATOR_HPP_ */
