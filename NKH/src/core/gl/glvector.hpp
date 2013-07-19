/*
 * glvector.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLVECTOR_HPP_
#define GLVECTOR_HPP_

#include "gltraits.hpp"
#include "glrequires.hpp"
#include <vector>
#include <iterator>

namespace nkh {
namespace core {
namespace gl {

template <class V, class It>
class gl_vector_iterator;

template<typename T, typename Buff = gl_buffer<T>, typename Alloc = gl_allocator<T, Buff> >
class gl_vector : protected std::vector<T, Alloc>
{
public:

    typedef std::vector<T, Alloc>                       base_vector_type;
    typedef typename base_vector_type::iterator         base_iterator_type;
    typedef typename base_vector_type::reverse_iterator
                                                        base_reverse_iterator_type;

    // ================================================================ //
    // ============================= TYPES ============================ //
    // ================================================================ //


    typedef typename base_vector_type::reference        reference;
    typedef typename base_vector_type::const_reference  const_reference;
    typedef gl_vector_iterator<gl_vector, base_iterator_type>
                                                      iterator;
    typedef typename base_vector_type::const_iterator   const_iterator;
    typedef typename base_vector_type::size_type        size_type;
    typedef typename base_vector_type::difference_type  difference_type;
    typedef T                                           value_type;
    typedef Alloc                                       allocator_type;
    typedef typename base_vector_type::pointer          pointer;
    typedef typename base_vector_type::const_pointer    const_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    explicit gl_vector(const Alloc& p_a = Alloc()):
            m_vector(p_a),
            m_buffer{0}
    {}

    explicit gl_vector(size_type p_n):
            m_vector(p_n),
            m_buffer{0}
    {}

    template<class InputIt>
    gl_vector(InputIt p_first, InputIt p_last, const Alloc& p_a = Alloc() ):
        m_vector(p_first, p_last, p_a),
        m_buffer{0}
    {}

    gl_vector(const gl_vector& rhs):
        m_vector(rhs.m_vector),
        m_buffer{rhs.m_buffer}
    {}

    ~gl_vector()
    {
        delete_buffer();
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    base_vector_type& base ()
    {
        // TODO load the vector from gpu memory.
        return m_vector;
    }
    const base_vector_type& base () const
    {
        // TODO load the vector from gpu memory.
        return m_vector;
    }

    /**
     * \brief Bind the underlying buffer to the current context.
     * This function use the templated target type.
     * \return True if the bind has succeed.
     */
    bool bind()
    {
        if(is_loaded() && glBindBuffer)
            glCheck(glBindBuffer(Buff::target, m_buffer));
        return is_loaded();
    }

    /**
     * \brief Load this buffer to the GPU memory.
     * This function try to load the buffer into the gpu memory.
     * If succeed, then client memory is free and the gl_vector can
     * be
     * \param p_usage is the usage hint for OpenGL.
     * \return True if succeed.
     */
    bool load_to_device(GLenum p_usage = GL_STATIC_DRAW)
    {
        if(is_loaded() == false)
        {
            // We create the buffer.
            create_buffer();

            if(bind())
            {
                try {
                    glBufferData(Buff::target, m_vector.size() * sizeof(T), m_vector.data(), p_usage);
                    priv::glTryError();
                    m_vector.clear();
                    return true;
                }
                catch(gl_out_of_memory& p_error)
                {
                    // TODO log the out of memory error.
                    return false;
                }
                catch(gl_exception& p_error)
                {
                    // TODO log the error.
                    return false;
                }
            }
            else
            {
                delete_buffer();
                return false;
            }
        }
        // The memory is already loaded.
        return true;
    }

private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Returns true if the memory has been loaded on the gpu.
     * \return true if the buffer id is a valid bufffer.
     */
    bool is_loaded() const
    {
        return m_buffer != 0;
    }

    void create_buffer()
    {
        glCheck(glGenBuffers(1, &m_buffer));
    }

    void delete_buffer()
    {
        glCheck(glDeleteBuffers(1, &m_buffer));
        m_buffer = 0;
    }

};


/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template <class T, class B, class A>
    inline bool operator==(const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () == y.base ();}

    template <class T, class B, class A>
    inline bool operator==(const gl_vector<T, B, A>& x,
                           const std::vector<T,A>& y)
    {return x.base () == y;}

    template <class T, class B, class A>
    inline bool operator==(const std::vector<T,A>& x,
                           const gl_vector<T, B, A>& y)
    {return x == y.base ();}

    template <class T, class B, class A>
    inline bool operator< (const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () < y.base ();}

    template <class T, class B, class A>
    inline bool operator<(const gl_vector<T, B, A>& x,
                          const std::vector<T,A>& y)
    {return x.base () < y;}

    template <class T, class B, class A>
    inline bool operator<(const std::vector<T,A>& x,
                          const gl_vector<T, B, A>& y)
    {return x < y.base ();}

    template <class T, class B, class A>
    inline bool operator!=(const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () != y.base ();}

    template <class T, class B, class A>
    inline bool operator!=(const gl_vector<T, B, A>& x,
                           const std::vector<T,A>& y)
    {return x.base () != y;}

    template <class T, class B, class A>
    inline bool operator!=(const std::vector<T,A>& x,
                           const gl_vector<T, B, A>& y)
    {return x != y.base ();}

    template <class T, class B, class A>
    inline bool operator> (const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () > y.base ();}

    template <class T, class B, class A>
    inline bool operator>=(const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () >= y.base ();}

    template <class T, class B, class A>
    inline bool operator>=(const gl_vector<T, B, A>& x,
                           const std::vector<T,A>& y)
    {return x.base () >= y;}

    template <class T, class B, class A>
    inline bool operator>=(const std::vector<T,A>& x,
                           const gl_vector<T, B, A>& y)
    {return x >= y.base ();}

    template <class T, class B, class A>
    inline bool operator<=(const gl_vector<T, B, A>& x,
                           const gl_vector<T, B, A>& y)
    {return x.base () <= y.base ();}

    template <class T, class B, class A>
    inline bool operator<=(const gl_vector<T, B, A>& x,
                           const std::vector<T,A>& y)
    {return x.base () <= y;}

    template <class T, class B, class A>
    inline bool operator<=(const std::vector<T,A>& x,
                           const gl_vector<T, B, A>& y)
    {return x <= y.base ();}


/**
 * \class gl_vector_iterator is an iterator for gl_vector.
 */
template<class V, class It>
class gl_vector_iterator
{
public :
    typedef V                                    vector_type;
    typedef It                                   base_iterator_type;
    typedef typename vector_type::const_iterator const_iterator_type;

    // ================================================================ //
    // ============================ TYPEDEF =========================== //
    // ================================================================ //

    typedef std::iterator_traits<It> gl_iterator_traits;

    typedef typename gl_iterator_traits::value_type         value_type;
    typedef typename gl_iterator_traits::difference_type    difference_type;
    typedef typename gl_iterator_traits::pointer            pointer;
    typedef typename gl_iterator_traits::reference          reference;
    typedef typename gl_iterator_traits::iterator_category  iterator_category;

    typedef typename vector_type::size_type         size_type;
    typedef typename vector_type::const_reference   const_reference;
    typedef typename vector_type::const_pointer     const_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /***
     * \brief Default constructor.
     */
    gl_vector_iterator():
        m_v{0},
        m_i{}
    {}

    gl_vector_iterator(vector_type* p_v, const base_iterator_type& p_i):
        m_v(p_v),
        m_i(p_i)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    operator const_iterator_type () const { return m_i; }

    base_iterator_type base() const  { return m_i; }
    vector_type* vector() const { return m_v; }

    const_reference operator* () const { return *m_i; }
    const_pointer operator->() const { return m_i.operator->(); }
    const_reference operator[] (difference_type p_n) const { return m_i[p_n]; }

    gl_vector_iterator& operator++ () { ++m_i; return *this; }
    gl_vector_iterator operator++(int) { return gl_vector_iterator(m_v, m_i++); }
    gl_vector_iterator& operator-- () { --m_i; return *this; }
    gl_vector_iterator operator-- (int) { return gl_vector_iterator(m_v, m_i--); }

    gl_vector_iterator operator+ (difference_type p_n) const { return gl_vector_iterator(m_v, m_i + p_n); }
    gl_vector_iterator& operator+= (difference_type p_n) { m_i += p_n; return *this; }
    gl_vector_iterator operator- (difference_type p_n) const { return gl_vector_iterator(m_v, m_i - p_n); }
    gl_vector_iterator& operator-= (difference_type p_n) { m_i -= p_n; return *this; }

    base_iterator_type _base () const {return m_i;} // Same as base ().

private:

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    vector_type* m_v;
    base_iterator_type m_i;
};

/**
 * \class Reverse iterator (Partial specialization)
 */
template<class V, class J>
class gl_vector_iterator<V, std::reverse_iterator<J> >
{
public:
    // ================================================================ //
    // ============================ TYPEDEF =========================== //
    // ================================================================ //

    typedef V                                               vector_type;
    typedef std::reverse_iterator<J>                        base_iterator_type;
    typedef typename vector_type::const_reverse_iterator    const_iterator_type;
    typedef std::iterator_traits<base_iterator_type>        base_iterator_traits;

    typedef typename vector_type::iterator                  iterator_type;
    typedef typename base_iterator_traits::value_type       value_type;
    typedef typename base_iterator_traits::difference_type  difference_type;
    typedef typename base_iterator_traits::pointer          pointer;
    typedef typename base_iterator_traits::reference        reference;
    typedef typename base_iterator_traits::iterator_category
                                                            iterator_category;

    typedef typename vector_type::size_type                 size_type;
    typedef typename vector_type::const_reference           const_reference;
    typedef typename vector_type::const_pointer             const_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_vector_iterator ():
        m_v (0),
        m_i ()
    {}

    explicit gl_vector_iterator (const iterator_type& i):
        m_v (i.vector ()),
        m_i (i.base ())
    {}

    gl_vector_iterator (vector_type* v, const base_iterator_type& i):
        m_v (v),
        m_i (i)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    operator const_iterator_type () const {return m_i;}
    iterator_type base () const {return iterator_type (m_v, m_i.base ());}
    base_iterator_type rbase () const {return m_i;}
    vector_type* vector () const {return m_v;}

    const_reference operator* () const {return *m_i;}
    const_pointer   operator-> () const {return m_i.operator -> ();}
    const_reference operator[] (difference_type p_n) const {return m_i[p_n];}

    gl_vector_iterator& operator++ () {++m_i; return *this;}
    gl_vector_iterator  operator++ (int) {return gl_vector_iterator (m_v, m_i++);}
    gl_vector_iterator& operator-- () {--m_i; return *this;}
    gl_vector_iterator  operator-- (int) {return gl_vector_iterator (m_v, m_i--);}

    gl_vector_iterator operator+ (difference_type p_n) const
    {return gl_vector_iterator (m_v, m_i + p_n);}
    gl_vector_iterator& operator+= (difference_type p_n) {m_i += p_n; return *this;}
    gl_vector_iterator operator- (difference_type p_n) const
    {return gl_vector_iterator (m_v, m_i - p_n);}
    gl_vector_iterator& operator-= (difference_type p_n) {m_i -= p_n; return *this;}

    base_iterator_type _base () const {return m_i;} // Same as rbase

private:

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    vector_type* m_v;
    base_iterator_type m_i;
};

/*
 * Overloaded operators.
 */

    /**
     * operator==
     */
    template <class V, class I>
    inline bool operator== (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () == y._base ();}

    template <class V, class I>
    inline bool operator== (const gl_vector_iterator<V, I>& x,
                            const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () == y;}

    template <class V, class I>
    inline bool operator== (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                            const gl_vector_iterator<V, I>& y)
    {return x == y._base ();}

    /**
     * operator<
     */
    template <class V, class I>
    inline bool operator< (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () < y._base ();}

    template <class V, class I>
    inline bool operator< (const gl_vector_iterator<V, I>& x,
                           const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () < y;}

    template <class V, class I>
    inline bool operator< (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                           const gl_vector_iterator<V, I>& y)
    {return x < y._base ();}

    /**
     * operator!=
     */
    template <class V, class I>
    inline bool operator!= (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () != y._base ();}

    template <class V, class I>
    inline bool operator!= (const gl_vector_iterator<V, I>& x,
                            const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () != y;}

    template <class V, class I>
    inline bool operator!= (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                            const gl_vector_iterator<V, I>& y)
    {return x != y._base ();}

    /**
     * operator>
     */
    template <class V, class I>
    inline bool operator> (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () > y._base ();}

    template <class V, class I>
    inline bool operator> (const gl_vector_iterator<V, I>& x,
                           const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () > y;}

    template <class V, class I>
    inline bool operator> (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                           const gl_vector_iterator<V, I>& y)
    {return x > y._base ();}

    /**
     * operator>=
     */
    template <class V, class I>
    inline bool operator>= (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () >= y._base ();}

    template <class V, class I>
    inline bool operator>= (const gl_vector_iterator<V, I>& x,
                            const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () >= y;}

    template <class V, class I>
    inline bool operator>= (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                            const gl_vector_iterator<V, I>& y)
    {return x >= y._base ();}

    /**
     * operator<=
     */
    template <class V, class I>
    inline bool operator<= (const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () <= y._base ();}

    template <class V, class I>
    inline bool operator<= (const gl_vector_iterator<V, I>& x,
                            const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () <= y;}

    template <class V, class I>
    inline bool operator<= (const typename gl_vector_iterator<V, I>::const_iterator_type& x,
                            const gl_vector_iterator<V, I>& y)
    {return x <= y._base ();}

    /**
     * operator-
     */
    template <class V, class I>
    inline typename gl_vector_iterator<V, I>::difference_type
    operator-(const gl_vector_iterator<V, I>& x, const gl_vector_iterator<V, I>& y)
    {return x._base () - y._base ();}

    template <class V, class I>
    inline typename gl_vector_iterator<V, I>::difference_type
    operator-(const gl_vector_iterator<V, I>& x,
              const typename gl_vector_iterator<V, I>::const_iterator_type& y)
    {return x._base () - y;}

    template <class V, class I>
    inline typename gl_vector_iterator<V, I>::difference_type
    operator-(const typename gl_vector_iterator<V, I>::const_iterator_type& x,
              const gl_vector_iterator<V, I>& y)
    {return x - y._base ();}

    /**
     * operator+
     */
    template <class V, class I>
    inline gl_vector_iterator<V, I>
    operator+(typename gl_vector_iterator<V, I>::difference_type n,
              const gl_vector_iterator<V, I>& x)
    {return gl_vector_iterator<V, I> (x.vector (), n + x._base ());}

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */

//namespace std
//{
//  template <class T, class B, class A>
//  inline void swap(nkh::core::gl::gl_vector<T, B, A>& x,
//                   nkh::core::gl::gl_vector<T, B, A>& y) {x.swap (y);}
//}

#include "glvector.inl"

#endif /* GLVECTOR_HPP_ */
