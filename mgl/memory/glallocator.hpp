/*
 * glallocator.hpp
 *
 *  Created on: 24 juin 2013
 *      Author: nemikolh
 */

#ifndef GLALLOCATOR_HPP_
#define GLALLOCATOR_HPP_

#include "glptr.hpp"

namespace mgl {

/**
 * @brief gl_allocator is the allocator.
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

    /** @brief Force non copy of the allocator. They must be preserved per Container.  */
    typedef std::false_type propagate_on_container_copy_assignment;
    /** @brief Force the non-move of the allocator. They must be preserved per Container.*/
    typedef std::false_type propagate_on_container_move_assignment;
    /** @brief Force the non-swap of the allocator. They must be preserved per Container. */
    typedef std::false_type propagate_on_container_swap;

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
     * @brief Default constructor.
     */
    explicit gl_allocator(Container* p_owner)
        : m_owner(p_owner)
    {}

    /**
     * @brief Default copy constructor.
     */
    gl_allocator(const gl_allocator& ) = default;

    /**
     * @brief Default move constructor.
     */
    gl_allocator(gl_allocator&&) = default;

    /**
     * @brief Copy constructor
     * @param p_rhs is the allocator we copy from.
     */
    template<typename T2, typename C2, typename B2>
    gl_allocator(const gl_allocator<T2, C2, B2>& p_rhs)
        : m_owner(p_rhs.m_owner)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief This function assumes that the allocate are only
     * made for the owner. Thus this one has a queue where allocate
     * push on top and deallocate pop on back.
     * @param p_n is the number of object to allocate.
     * @param p_cptr is a conventional argument. Not used in the present code.
     * @return a new pointer that the owner will represent.
     */
    pointer allocate(size_type p_n, const_pointer /*p_cptr*/ = nullptr)
    {
        pointer _ret = nullptr;

        if (p_n > this->max_size())
            throw std::bad_alloc();

        // We put on the back of queue a new
        m_owner->push_address();
        gl_object_buffer<Buff>::gl_gen(1, &(m_owner->current_address().id));

        gl_object_buffer<Buff>::gl_bind(m_owner->id());
        gl_object_buffer<Buff>::gl_buffer_data(p_n * sizeof(T), nullptr);
        m_owner->map_pointer_range(0, p_n);
        _ret.set_base_address(&(m_owner->current_address()));

        return _ret;
    }

    /**
     * @brief Pop the back of the queue.
     * @param p_ptr is a pointer to the back of the queue.
     * @param p_n is the size of the buffer deallocated.
     */
    void deallocate(pointer p_ptr, size_type /*p_n*/)
    {
        // We pop the old address.
        auto old_address = m_owner->pop_address();
        assert(p_ptr.m_ptr != nullptr &&
               p_ptr.m_ptr->id == old_address.id &&
               p_ptr.m_ptr->ptr == old_address.ptr);

        // We delete the underlying buffer.
        gl_object_buffer<Buff>::gl_delete(1, &old_address.id);
        p_ptr.m_ptr = nullptr;
    }

    size_type max_size() const
    {
        return size_t(-1) / sizeof(T);
    }

private:

    // ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

    template<typename T1, typename C1, typename B1, typename T2, typename C2, typename B2>
    friend bool operator== (const gl_allocator<T1, C1, B1>&, const gl_allocator<T2, C2, B2>&);

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The owner of this instance of allocator. */
    Container* m_owner;
};

/*
 * Overloaded operators.
 */

    /**
     * @brief operator== for the gl_allocator type.
     *
     * Returns true if and only if, the gl_allocators have been created by the same gl_vector.
     */
    template<typename T, typename C1, typename B1, typename U, typename C2, typename B2>
    inline bool operator==(const gl_allocator<T, C1, B1>&, const gl_allocator<U, C2, B2>&)
    {
        // TODO: Check usage too ?
        return B1::target == B2::target;
    }

    /**
     * @brief operator != for the gl_allocator type.
     */
    template<typename T, typename C1, typename B1, typename U, typename C2, typename B2>
    inline bool operator!=(const gl_allocator<T, C1, B1>& p_a, const gl_allocator<U, C2, B2>& p_b)
    {
        return !(p_a == p_b);
    }

} /* namespace mgl */

#endif /* GLALLOCATOR_HPP_ */
