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

namespace nkh {
namespace core {
namespace gl {

template<typename T, typename Buff, typename Alloc>
class gl_vector
{
public:

    typedef std::vector<T, Alloc>                       base_vector_type;

    // ================================================================ //
    // ============================= TYPES ============================ //
    // ================================================================ //

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
    typedef Alloc                                           allocator_type;

    // TODO check the correct behaviour of the reverse_iterator with the custom iterator.
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef std::reverse_iterator<iterator>        reverse_iterator;

    //** Typedef for the automatic state saving. */
    //typedef gl_object_buffer gl_object_type;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_vector()
        : m_vector()
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
    }

    explicit gl_vector(const allocator_type& p_a = allocator_type())
        : m_vector(p_a)
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
    }

    explicit gl_vector(size_type p_n)
        : m_vector(p_n)
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
    }

    gl_vector(size_type p_n, const value_type& p_value, const allocator_type& p_a = allocator_type())
        : m_vector(p_n, p_value, p_a)
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
    }

    template<class InputIt>
    gl_vector(InputIt p_first, InputIt p_last, const Alloc& p_a = Alloc() )
        : m_vector(p_first, p_last, p_a)
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
    }

    gl_vector(const gl_vector& p_rhs)
        : m_vector(map_vector(p_rhs))
        , m_mapped(0)
    {
        get_ptr_impl().unmap();
        p_rhs.unmap();
    }


    gl_vector(gl_vector && p_rhs)
        : m_vector(std::move(p_rhs.m_vector))
        , m_mapped(std::move(p_rhs.m_mapped))
    {}

    gl_vector(std::initializer_list<value_type> p_l,
       const allocator_type& p_a = allocator_type())
        : m_vector(p_l, p_a)
        , m_mapped(0)
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
        get_ptr_impl().bind();
    }

    /**
     * \brief Unbind the buffer.
     */
    void unbind() const
    {

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
    begin()         {   return m_vector.begin();    }

    const_iterator
    begin() const   {   return m_vector.begin();    }

    iterator
    end()           {   return m_vector.end();      }

    const_iterator
    end() const     {   return m_vector.end();      }

    const_iterator
    cbegin() const  {   return m_vector.cbegin();   }

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
    operator[](size_type p_n) { return m_vector[p_n];   }

    const_reference
    operator[](size_type p_n) const { return m_vector[p_n]; }

    reference
    at(size_type p_n)       { return m_vector.at(p_n);  }

    const_reference
    at(size_type p_n) const { return m_vector.at(p_n);  }

    reference
    front()                 { return m_vector.front();  }

    const_reference
    front() const           { return m_vector.front();  }

    reference
    back()                  { return m_vector.back();   }

    const_reference
    back()  const           { return m_vector.back();   }

    T*
    data()                  { return m_vector.data();   }

    const T*
    data()  const           { return m_vector.data();   }

    void
    push_back(const value_type& p_val)
    {
        m_vector.push_back(p_val);
    }

    void
    push_back(value_type&& p_val)
    {
        m_vector.push_back(std::move(p_val));
    }

    template<typename... Args>
    void
    emplace_back(Args&&... p_args)
    {
        m_vector.emplace_back(std::forward<Args>(p_args)...);
    }

    void
    pop_back()              { m_vector.pop_back();      }

    template<typename... Args>
    iterator
    emplace(iterator p_position, Args&&... p_args)
    {
        m_vector.emplace(p_position, std::forward<Args>(p_args)...);
    }

    iterator
    insert(iterator p_position, const value_type& p_x)
    {
        return m_vector.insert(p_position, p_x);
    }

    iterator
    insert(iterator p_position, value_type&& p_x)
    {
        return m_vector.insert(p_position, std::move(p_x));
    }

    void
    insert(iterator p_position, std::initializer_list<value_type> p_list)
    {
        m_vector.insert(p_position, p_list);
    }

    void
    insert(iterator p_position, size_type p_n, const value_type& p_x)
    {
        m_vector.insert(p_position, p_n, p_x);
    }

    template<typename InputIterator>
    void
    insert(iterator p_position, InputIterator p_first, InputIterator p_last)
    {
        m_vector.insert(p_position, p_first, p_last);
    }

    iterator
    erase(iterator p_position)
    {
        return m_vector.erase(p_position);
    }

    iterator
    erase(iterator p_first, iterator p_last)
    {
        return m_vector.erase(p_first, p_last);
    }

    void
    swap(gl_vector& p_x)
    {
        using std::swap;
        m_vector.swap(p_x.m_vector);
        swap(m_mapped, p_x.m_mapped);
    }

    void
    clear()     { m_vector.clear(); }


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
    template<typename, typename, typename> friend class gl_vector;
    //friend gl_vector;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Map the vector.
     */
    void map() const
    {
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

    /** underlying vector. */
    base_vector_type        m_vector;
    /** the mapping state. */
    mutable unsigned int    m_mapped;
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

template<typename T, typename B, typename A>
class gl_scope<gl_vector<T, B, A>>
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Construct a scope object to map at scope the passed gl_vector.
     * @param p_vector is the instance object that will be mapped.
     */
    gl_scope(const gl_vector<T, B, A> & p_vector)
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

    const gl_vector<T, B, A>& m_obj;
};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template <class T, class B, class A>
    inline bool operator==(const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
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

    template <class T, class B, class A>
    inline bool operator< (const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
    {
        gl_scope<gl_vector<T, B, A> > binder_x{p_x};
        gl_scope<gl_vector<T, B, A> > binder_y{p_y};
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

    template <class T, class B, class A>
    inline bool operator!=(const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
    {
        gl_scope<gl_vector<T, B, A> > binder_x{p_x};
        gl_scope<gl_vector<T, B, A> > binder_y{p_y};
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

    template <class T, class B, class A>
    inline bool operator> (const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
    {
        gl_scope<gl_vector<T, B, A> > binder_x{p_x};
        gl_scope<gl_vector<T, B, A> > binder_y{p_y};
        return p_x.base () > p_y.base ();
    }

    template <class T, class B, class A>
    inline bool operator>=(const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
    {
        gl_scope<gl_vector<T, B, A> > binder_x{p_x};
        gl_scope<gl_vector<T, B, A> > binder_y{p_y};
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

    template <class T, class B, class A>
    inline bool operator<=(const gl_vector<T, B, A>& p_x,
                           const gl_vector<T, B, A>& p_y)
    {
        gl_scope<gl_vector<T, B, A> > binder_x{p_x};
        gl_scope<gl_vector<T, B, A> > binder_y{p_y};
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

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */


#include "glvector.inl"

#endif /* GLVECTOR_HPP_ */
