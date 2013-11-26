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
#include "meta/gltraits.hpp"
#include "glrequires.hpp"
#include "glallocator.hpp"
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
        : m_id(0)
        , m_mapped(0)
        , m_vector(allocator_type(*this))
    {}

    explicit gl_vector(size_type p_n)
        : m_id(0)
        , m_mapped(0)
        , m_vector(p_n)
    {
        get_ptr_impl().unmap();
    }

    gl_vector(size_type p_n, const value_type& p_value)
        : m_id(0)
        , m_mapped(0)
        , m_vector(p_n, p_value,  allocator_type(*this))
    {
        get_ptr_impl().unmap();
    }

    template<class InputIt>
    gl_vector(InputIt p_first, InputIt p_last)
        : m_id(0)
        , m_mapped(0)
        , m_vector(p_first, p_last, allocator_type(*this))
    {
        get_ptr_impl().unmap();
    }

    gl_vector(const gl_vector& p_rhs)
        : m_id(0)
        , m_mapped(0)
        , m_vector(map_vector(p_rhs))
    {
        get_ptr_impl().unmap();
        p_rhs.unmap();
    }


    gl_vector(gl_vector && p_rhs)
        : m_id(std::move(p_rhs.m_id))
        , m_mapped(std::move(p_rhs.m_mapped))
        , m_vector(std::move(p_rhs.m_vector))
    {}

    gl_vector(std::initializer_list<value_type> p_l)
        : m_id(0)
        , m_mapped(0)
        , m_vector(p_l, allocator_type(*this))
    {
        get_ptr_impl().unmap();
    }

    ~gl_vector()
    {
        // We map because the destructor of the vector will call
        // the destructors on the elements and deallocate with the allocator
        // which do the unbind plus the deletion of the buffer.
        map();
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Bind the underlying buffer.
     */
    void bind() const
    {
        gl_object_buffer<Buff>::gl_bind(m_id);
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
        m_id     = std::move(p_rhs.m_id);
        m_vector = std::move(p_rhs.m_vector);
        m_mapped = std::move(p_rhs.m_mapped);
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
        map();
        m_vector.reserve(p_n);
        unmap();
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

    base_vector_type map_vector(const gl_vector& p_rhs)
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
        assert(m_id);
#endif
        if(!m_mapped)
        {
            bind();
            get_ptr_impl().map_range(0, m_vector.capacity());
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
            get_ptr_impl().unmap();
        }
    }

    pointer get_ptr_impl() const
    {
        return m_vector.cbegin().base();
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** The id of the buffer. */
    GLuint                  m_id;
    /** the mapping state. */
    mutable unsigned int    m_mapped;
    /** underlying vector. */
    base_vector_type        m_vector;
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

    // TODO rework those, as the std::vector can use an other allocator.
//    template <class T, class B, class A>
//    inline bool operator==(const gl_vector<T, B, A>& x,
//                           const std::vector<T,A>& y)
//    {return x.base () == y;}
//
//    template <class T, class B, class A>
//    inline bool operator==(const std::vector<T,A>& x,
//                           const gl_vector<T, B, A>& y)
//    {return x == y.base ();}

    template <class T, class B>
    inline bool operator< (const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () < p_y.base ();
    }

//    template <class T, class B, class A>
//    inline bool operator<(const gl_vector<T, B, A>& x,
//                          const std::vector<T,A>& y)
//    {return x.base () < y;}
//
//    template <class T, class B, class A>
//    inline bool operator<(const std::vector<T,A>& x,
//                          const gl_vector<T, B, A>& y)
//    {return x < y.base ();}

    template <class T, class B>
    inline bool operator!=(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () != p_y.base ();
    }

//    template <class T, class B, class A>
//    inline bool operator!=(const gl_vector<T, B, A>& x,
//                           const std::vector<T,A>& y)
//    {return x.base () != y;}
//
//    template <class T, class B, class A>
//    inline bool operator!=(const std::vector<T,A>& x,
//                           const gl_vector<T, B, A>& y)
//    {return x != y.base ();}

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

//    template <class T, class B, class A>
//    inline bool operator>=(const gl_vector<T, B, A>& x,
//                           const std::vector<T,A>& y)
//    {return x.base () >= y;}
//
//    template <class T, class B, class A>
//    inline bool operator>=(const std::vector<T,A>& x,
//                           const gl_vector<T, B, A>& y)
//    {return x >= y.base ();}

    template <class T, class B>
    inline bool operator<=(const gl_vector<T, B>& p_x,
                           const gl_vector<T, B>& p_y)
    {
        gl_scope<gl_vector<T, B> > binder_x(p_x);
        gl_scope<gl_vector<T, B> > binder_y(p_y);
        return p_x.base () <= p_y.base ();
    }

//    template <class T, class B, class A>
//    inline bool operator<=(const gl_vector<T, B, A>& x,
//                           const std::vector<T,A>& y)
//    {return x.base () <= y;}
//
//    template <class T, class B, class A>
//    inline bool operator<=(const std::vector<T,A>& x,
//                           const gl_vector<T, B, A>& y)
//    {return x <= y.base ();}

} /* namespace mgl */


#endif /* GLVECTOR_HPP_ */
