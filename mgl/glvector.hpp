/*
 * glvector.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLVECTOR_HPP_
#define GLVECTOR_HPP_

#include <vector>
#include <iterator>
#include <queue>
#include "type/gltraits.hpp"
#include "glrequires.hpp"
#include "memory/glallocator.hpp"
#include "glscope.hpp"

namespace mgl {

template<typename T, typename Buff>
class gl_vector
{
public:

    // ================================================================ //
    // ============================= TYPES ============================ //
    // ================================================================ //

    typedef gl_allocator<T, gl_vector<T, Buff>, Buff>       allocator_type;
    typedef std::vector<T, allocator_type>                  base_vector_type;

    typedef typename base_vector_type::pointer              pointer;
    typedef typename base_vector_type::const_pointer        const_pointer;
    typedef typename base_vector_type::reference            reference;
    typedef typename base_vector_type::const_reference      const_reference;

    typedef typename base_vector_type::iterator             iterator;
    typedef typename base_vector_type::const_iterator       const_iterator;
    //typedef gl_vector_iterator<pointer, gl_vector>          iterator;
    //typedef gl_vector_iterator<const_pointer, gl_vector>    const_iterator;
    typedef typename pointer::size_type                     size_type;
    typedef typename pointer::difference_type               difference_type;
    typedef T                                               value_type;

    // TODO check the correct behaviour of the reverse_iterator with the custom iterator.
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef std::reverse_iterator<iterator>        reverse_iterator;

    //** Typedef for the automatic state saving. */
    //typedef gl_object_buffer gl_object_type;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    explicit gl_vector()
        : m_gpu_buff_stack()
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(allocator_type(*this))
    {}

    explicit gl_vector(size_type p_n)
        : m_gpu_buff_stack()
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(p_n)
    {
        unmap_pointer();
    }

    gl_vector(size_type p_n, const value_type& p_value)
        : m_gpu_buff_stack()
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(p_n, p_value,  allocator_type(*this))
    {
        unmap_pointer();
    }

    template<class InputIt>
    gl_vector(InputIt p_first, InputIt p_last)
        : m_gpu_buff_stack()
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(p_first, p_last, allocator_type(*this))
    {
        unmap_pointer();
    }

    gl_vector(const gl_vector& p_rhs)
        : m_gpu_buff_stack()
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(map_vector(p_rhs), allocator_type(*this))
    {
        unmap_pointer();
        p_rhs.unmap();
    }


    gl_vector(gl_vector && p_rhs)
        : m_gpu_buff_stack(std::move(p_rhs.m_gpu_buff_stack))
        , m_mapped(std::move(p_rhs.m_mapped))
#ifndef NKH_NDEBUG
        , m_map_ranged_called(std::move(m_map_ranged_called))
#endif
        , m_vector(std::move(p_rhs.m_vector), allocator_type(*this))
    {}

    gl_vector(std::initializer_list<value_type> p_l)
        : m_gpu_buff_stack()//{0, nullptr}
        , m_mapped(0)
#ifndef NKH_NDEBUG
        , m_map_ranged_called(false)
#endif
        , m_vector(p_l, allocator_type(*this))
    {
        unmap_pointer();
    }

    ~gl_vector()
    {
        // We map because the destructor of the vector will call
        // the destructors on the elements and deallocate with the allocator
        // which do the unbind plus the deletion of the buffer.
        clear();
        gl_object_buffer<Buff>::gl_delete(1, id_ptr());
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Bind the underlying buffer.
     */
    void bind() const
    {
        gl_object_buffer<Buff>::gl_bind(current_address().id);
    }

    gl_vector&
    operator=(const gl_vector& p_rhs)
    {
        p_rhs.map();
        map();
        m_vector = p_rhs.m_vector;
        unmap();
        p_rhs.unmap();
        return *this;
    }

    gl_vector&
    operator=(gl_vector&& p_rhs)
    {
        m_gpu_buff_stack  = std::move(p_rhs.m_gpu_buff_stack);
        m_mapped        = std::move(p_rhs.m_mapped);
#ifndef NKH_NDEBUG
        m_map_ranged_called = std::move(p_rhs.m_map_ranged_called);
#endif
        m_vector        = std::move(p_rhs.m_vector);
        return *this;
    }

    gl_vector&
    operator=(std::initializer_list<value_type> p_l)
    {
        map();
        m_vector = p_l;
        unmap();
        return *this;
    }

    void inline assign(size_type p_n, const value_type& p_val)
    {
        map();
        m_vector.assign(p_n, p_val);
        unmap();
    }

    template<typename Iterator>
    void inline assign(Iterator p_first, Iterator p_last)
    {
        map();
        m_vector.assign(p_first, p_last);
        unmap();
    }

    void inline assign(std::initializer_list<value_type> p_list)
    {
        map();
        m_vector.assign(p_list);
        unmap();
    }

    allocator_type get_allocator() const
    {
        return m_vector.get_allocator();
    }

    iterator
    begin()
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.begin();
    }

    const_iterator
    begin() const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.begin();
    }

    iterator
    end()           {   return m_vector.end();      }

    const_iterator
    end() const     {   return m_vector.end();      }

    const_iterator
    cbegin() const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.cbegin();
    }

    const_iterator
    cend() const    {   return m_vector.cend();     }

    size_type
    size() const    {   return m_vector.size();     }

    size_type
    max_size() const {  return m_vector.max_size(); }

    void
    resize(size_type p_n)
    {
        map();
        m_vector.resize(p_n);
        unmap();
    }

    void
    resize(size_type p_n, const value_type& p_val)
    {
        map();
        m_vector.resize(p_n, p_val);
        unmap();
    }

    void
    shrink_to_fit()
    {
        map();
        m_vector.shrink_to_fit();
        unmap();
    }

    size_type
    capacity() const        { return m_vector.capacity();   }

    bool
    empty() const           { return m_vector.empty();      }

    void
    reserve(size_type p_n)
    {
        m_vector.reserve(p_n);
        unmap_pointer();
    }

    const base_vector_type&
    base() const            { return m_vector;          }

    reference
    operator[](size_type p_n)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector[p_n];
    }

    const_reference
    operator[](size_type p_n) const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector[p_n];
    }

    reference
    at(size_type p_n)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.at(p_n);
    }

    const_reference
    at(size_type p_n) const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.at(p_n);
    }

    reference
    front()
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.front();
    }

    const_reference
    front() const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.front();
    }

    reference
    back()
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.back();
    }

    const_reference
    back()  const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.back();
    }

    T*
    data()
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.data();
    }

    const T*
    data()  const
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.data();
    }

    void
    push_back(const value_type& p_val)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.push_back(p_val);
    }

    void
    push_back(value_type&& p_val)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.push_back(std::move(p_val));
    }

    template<typename... Args>
    void
    emplace_back(Args&&... p_args)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.emplace_back(std::forward<Args>(p_args)...);
    }

    void
    pop_back()
    {
        map();
        m_vector.pop_back();
        unmap();
    }

    template<typename... Args>
    iterator
    emplace(iterator p_position, Args&&... p_args)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.emplace(p_position, std::forward<Args>(p_args)...);
    }

    iterator
    insert(iterator p_position, const value_type& p_x)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.insert(p_position, p_x);
    }

    iterator
    insert(iterator p_position, value_type&& p_x)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.insert(p_position, std::move(p_x));
    }

    void
    insert(iterator p_position, std::initializer_list<value_type> p_list)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.insert(p_position, p_list);
    }

    void
    insert(iterator p_position, size_type p_n, const value_type& p_x)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.insert(p_position, p_n, p_x);
    }

    template<typename InputIterator>
    void
    insert(iterator p_position, InputIterator p_first, InputIterator p_last)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        m_vector.insert(p_position, p_first, p_last);
    }

    iterator
    erase(iterator p_position)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.erase(p_position);
    }

    iterator
    erase(iterator p_first, iterator p_last)
    {
#       ifndef NKH_NDEBUG
        assert(m_mapped);
#       endif
        return m_vector.erase(p_first, p_last);
    }

    void
    swap(gl_vector& p_x)
    {
        map();
        p_x.map();
        using std::swap;
        m_vector.swap(p_x.m_vector);
        swap(m_mapped, p_x.m_mapped);
        unmap();
        p_x.unmap();
    }

    void
    clear()
    {
        map();
        m_vector.clear();
        unmap();
    }


private:

    // ================================================================ //
    // ============================ HELPERS =========================== //
    // ================================================================ //

    const base_vector_type& map_vector(const gl_vector& p_rhs)
    {
        p_rhs.map();
        return p_rhs.m_vector;
    }

    // ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

    template<typename, typename> friend class gl_vector_iterator;
    template<typename> friend class gl_scope;
    template<typename, typename> friend class gl_vector;
    friend allocator_type;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Map the vector.
     */
    void map() const
    {
#ifndef NKH_DEBUG
        //assert(!m_gpu_buff_stack.empty() && current_address().id);
#endif
        if(!m_gpu_buff_stack.empty() && !m_mapped)
        {
            bind();
            //auto len = max(m_vector.capacity(), 1);
            map_pointer_range(0, m_vector.capacity());
        }
        ++m_mapped;
    }

    /**
     * \brief Unmap the vector.
     */
    void unmap() const
    {
#ifndef NKH_DEBUG
        assert(m_mapped > 0);
#endif
        --m_mapped;
        if(m_mapped == 0)
        {
            bind();
            unmap_pointer();
        }
    }

    /**
     * \brief Map the pointer to be a valid pointer.
     * Careful ! When using this function, the user should take care of saving
     * any previously bound Buffer, and bound the buffer hold by this gl_ptr.
     * Furthermore, if p_length + p_offset is superior to the size of the buffer
     * then error will rise.
     * \param p_offset is the offset for the range.
     * \param p_length is the number of element to take into account.
     */
    void map_pointer_range(difference_type p_offset, size_type p_length) const
    {
        //assert(p_length > 0);
        // We make the assumption than the buffer content isn't used in draw call, that's why we have the GL_MAP_UNSYNCHRONIZED_BIT flag
        // And finally, because of the static_assert, the cast can't fail.
        current_address().ptr = reinterpret_cast<T*>(
                gl_object_buffer<Buff>::gl_map_range(p_offset * sizeof(T),
                                                     p_length * sizeof(T),
                                                     GL_MAP_WRITE_BIT | GL_MAP_READ_BIT/*| GL_MAP_UNSYNCHRONIZED_BIT*/));
#ifndef NKH_NDEBUG
        m_map_ranged_called = true;
        check_integrity();
#endif
    }

    /**
     * \brief Unmap the underlying buffer.
     * Careful ! The buffer isn't manually bound here. This function
     * made the assumption than the user has done it with bind(), and
     * has saved any previously bound buffers.
     */
    void unmap_pointer() const
    {
#ifndef NKH_DEBUG
        assert(m_map_ranged_called);
        m_map_ranged_called = false;
#endif
        assert(gl_object_buffer<Buff>::gl_unmap());
        glCheck(current_address().ptr = nullptr);
    }

#ifndef NKH_NDEBUG
    /**
     * \brief Check for the pointer validity.
     * This function make the pointer valid if it wasn't.
     */
    void check_integrity() const
    {
        // Rude for now, but will force the code to be ok.
        assert(priv::glCheckError(__FILE__, __LINE__));
    }
#endif

    inline GLuint* id_ptr()
    {
        return &current_address().id;
    }

    inline GLuint id() const
    {
        return current_address().id;
    }

    inline void reset_id()
    {
        current_address().id = 0;
    }

    inline gpu_buffer<T>& current_address() const
    {
        return m_gpu_buff_stack.back();
    }

    inline gpu_buffer<T>& current_address()
    {
        return m_gpu_buff_stack.back();
    }

    inline void push_address()
    {
        m_gpu_buff_stack.push({0, nullptr});
    }

    inline gpu_buffer<T> pop_address()
    {
        auto old = m_gpu_buff_stack.front();
        m_gpu_buff_stack.pop();
        return old;
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The base address for the mapping. */
    mutable std::queue<gpu_buffer<T>>   m_gpu_buff_stack;
    /** the mapping state. */
    mutable unsigned int                m_mapped;
#ifndef NKH_NDEBUG
    mutable bool                        m_map_ranged_called;
#endif
    /** underlying vector. */
    base_vector_type                    m_vector;
};

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
    template<typename U>
    friend gl_instanced<U> make_instanced(const gl_vector<U>& p_buffer);

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Bind the instanced buffer.
     */
    inline void bind()
    {
        m_buffer.bind();
    }

    /**
     * \brief Returns the divisor number.
     * Currently returns just 1. That implies having
     * the buffer being modified every one instance.
     * \return Returns the divisor number
     */
    inline char get_divisor()
    {
        return 1;
    }

private:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_instanced(const gl_vector<T>& p_buffer)
        : m_buffer{p_buffer}
    {}

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The buffer that will behave like instanced data. */
    const gl_vector<T>& m_buffer;
};

template<typename T>
gl_instanced<T> make_instanced(const gl_vector<T>& p_buffer)
{
    return gl_instanced<T>(p_buffer);
}

template<typename T, typename B>
class gl_scope<gl_vector<T, B>>
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Construct a scope object to map at scope the passed gl_vector.
     * The vector must correspond to an openGL buffer.
     * @param p_vector is the instance object that will be mapped.
     */
    gl_scope(const gl_vector<T, B> & p_vector)
        : m_obj(p_vector)
    {
        m_obj.map();
    }

    /**
     * @brief Destructor, unmap the underlying vector.
     */
    ~gl_scope()
    {
        m_obj.unmap();
    }

private:

    const gl_vector<T, B>& m_obj;
};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template <class T, class B>
    inline bool operator==(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        auto binder_x = bind_at_scope(p_x);
        auto binder_y = bind_at_scope(p_y);
        return p_x.base () == p_y.base ();
    }

    template <class T, class B>
    inline bool operator< (const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () < p_y.base ();
    }

    template <class T, class B>
    inline bool operator!=(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () != p_y.base ();
    }

    template <class T, class B>
    inline bool operator> (const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () > p_y.base ();
    }

    template <class T, class B>
    inline bool operator>=(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () >= p_y.base ();
    }

    template <class T, class B>
    inline bool operator<=(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () <= p_y.base ();
    }

} /* namespace mgl */


#endif /* GLVECTOR_HPP_ */
