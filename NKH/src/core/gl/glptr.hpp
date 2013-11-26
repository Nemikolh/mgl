/*
 * glptr.h
 *
 *  Created on: 13 avr. 2013
 *      Author: nemikolh
 */

#ifndef GLPTR_H_
#define GLPTR_H_

#include <type_traits>
#include <assert.h>
#include <memory>
#include "glfwd.hpp"
#include "meta/gltraits.hpp"

namespace mgl {

namespace priv {

template<class T, typename Buff>
class gl_ptr_impl
{
public:

    // ================================================================ //
    // ============================ TYPEDEF =========================== //
    // ================================================================ //

    typedef T                               value_type;
    typedef ptrdiff_t                       difference_type;
    typedef T *                             pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef size_t                          size_type;
    typedef gl_ptr<void>                    void_pointer;
    typedef const gl_ptr<void>              const_void_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default Constructor.
     */
	gl_ptr_impl(std::nullptr_t = nullptr)
	    : m_ptr(std::make_shared<pointer>(nullptr))
	    , m_offset(0)
        #ifndef NKH_NDEBUG
	    , m_isMap(std::make_shared<bool>(false))
        #endif
    {}

    /**
     * \brief Copy constructor
     * \param rhs is an other gl_ptr.
     */
    gl_ptr_impl(const gl_ptr_impl& rhs)
        : m_ptr(rhs.m_ptr)
        , m_offset(rhs.m_offset)
        #ifndef NKH_NDEBUG
        , m_isMap(rhs.m_isMap)
        #endif
    {}

	/**
	 * \brief Copy Constructor.
	 * \param rhs is a gl_ptr where U is convertible into T.
	 */
    template<typename U, typename B, typename = typename
            std::enable_if<std::is_convertible<U*, T*>::value>::type>
	gl_ptr_impl(const gl_ptr_impl<U, B>& rhs)
	    : m_ptr(rhs.m_ptr)  // why not static_cast? -> to avoid accepting downcasting. Actually is_convertible wouldn't accept it.
	    , m_offset(rhs.m_offset)
        #ifndef NKH_NDEBUG
        , m_isMap(rhs.m_isMap)
        #endif
	{}

	/**
	 * \brief Move constructor.
	 * \param rhs the moved value.
	 */
	gl_ptr_impl(gl_ptr_impl && rhs)
        : m_ptr(std::move(rhs.m_ptr))
        , m_offset(std::move(rhs.m_offset))
        #ifndef NKH_NDEBUG
        , m_isMap(std::move(rhs.m_isMap))
        #endif
	{}

	/**
     * \brief Move constructor.
     * \param rhs the moved value.
     */
    template<typename U, typename B, typename = typename
            std::enable_if<std::is_convertible<U*, T*>::value>::type>
    gl_ptr_impl(gl_ptr_impl<U, B>&& rhs)
        : m_ptr(std::move(rhs.m_ptr))
        , m_offset(std::move(rhs.m_offset))
        #ifndef NKH_NDEBUG
        , m_isMap(std::move(rhs.m_isMap))
        #endif
    {}


	gl_ptr_impl& operator= (const gl_ptr_impl& rhs)
	{
	    m_ptr = rhs.m_ptr;
	    m_offset = rhs.m_offset;
#ifndef NKH_NDEBUG
	    m_isMap = rhs.m_isMap;
#endif
	    return *this;
	}

	gl_ptr_impl& operator= (std::nullptr_t)
	{
	    m_ptr = nullptr;
	    m_offset = 0;
#ifndef NKH_NDEBUG
        *m_isMap = false;
#endif
        return *this;
	}

	gl_ptr_impl& operator= (gl_ptr_impl&& p_rhs)
	{
	    m_ptr = std::move(p_rhs.m_ptr);
	    m_offset = std::move(p_rhs.m_offset);
#ifndef NKH_NDEBUG
        m_isMap = std::move(p_rhs.m_isMap);
#endif
	    return *this;
	}

	/**
	 * \brief Pointer interface.
	 * \return the underlying pointer.
	 */
	pointer     operator->() const
	{
#ifndef NKH_NDEBUG
        if(!*m_isMap)
            throw gl_exception("Pointer access not mapped");
#endif
	    return (*m_ptr+m_offset);
	}

	gl_ptr_impl&     operator++()
    {
	    ++m_offset;
	    return *this;
    }

	gl_ptr_impl      operator++(int)
    {
	    return gl_ptr_impl(m_offset++, *this);
    }

    /**
     *  Bidirectional iterator requirements
     */
	gl_ptr_impl&     operator--()
    {
	    --m_offset;
	    return *this;
    }

	gl_ptr_impl      operator--(int)
    {
	    return gl_ptr_impl(m_offset--, *this);
    }


    gl_ptr_impl&     operator+=(const difference_type& p_n)
    {
        m_offset += p_n; return *this;
    }

    gl_ptr_impl      operator+(const difference_type& p_n) const
    {
        return gl_ptr_impl(m_offset + p_n, *this);
    }

    gl_ptr_impl&     operator-=(const difference_type& p_n)
    {
        m_offset -= p_n; return *this;
    }

    gl_ptr_impl      operator-(const difference_type& p_n) const
    {
        return gl_ptr_impl(m_offset - p_n, *this);
    }

    /**
     * \brief Return true if the pointer is mapped and allocated.
     * Note : This is only true when NKH_NDEBUG isn't defined.
     * This can lead to weird behavior when this macro isn't define
     * if you have defined the NDEBUG macro.
     * \return true if the pointer is non null.
     */
    explicit operator bool() const
    {
#ifndef NKH_NDEBUG
        assert(*m_isMap);
        return *m_isMap && *m_ptr != nullptr;
#else
        return *m_ptr != nullptr;
#endif
    }

//    /**
//     * \brief Set the context for this pointer.
//     * \param p_context is the context.
//     */
//    void set_context(context p_context)
//    {
//        m_context = p_context;
//    }

protected:

    gl_ptr_impl(difference_type p_offset, const gl_ptr_impl & p_other)
        : m_ptr{p_other.m_ptr}
        , m_offset{p_offset}
#ifndef NKH_NDEBUG
        , m_isMap{p_other.m_isMap}
#endif
//        , m_context{p_other.m_context}
    {}

	// ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

	/**
	 * \brief Map the pointer to be a valid pointer.
	 * Careful ! When using this function, the user should take care of saving
	 * any previously bound Buffer, and bound the buffer hold by this gl_ptr.
	 * Furthermore, if p_length + p_offset is superior to the size of the buffer
	 * then error will rise.
	 * \param p_offset is the offset for the range.
	 * \param p_length is the number of element to take into account.
	 */
	void map_range(difference_type p_offset, size_type p_length)
	{
#ifndef NKH_NDEBUG
	    *m_isMap = true;
#endif
	    // We make the assumption than the buffer content isn't used in draw call, that's why we have the GL_MAP_UNSYNCHRONIZED_BIT flag
	    // And finally, because of the static_assert, the cast can't fail.
        *m_ptr = reinterpret_cast<T*>(
	            gl_object_buffer<Buff>::gl_map_range(p_offset * sizeof(T),
	                                                 p_length * sizeof(T),
	                                                 GL_MAP_WRITE_BIT | GL_MAP_READ_BIT/*| GL_MAP_UNSYNCHRONIZED_BIT*/));
#ifndef NKH_NDEBUG
	    check_integrity();
#endif
	}

	/**
	 * \brief Unmap the underlying buffer.
	 * Careful ! The buffer isn't manually bound here. This function
	 * made the assumption than the user has done it with bind(), and
	 * has saved any previously bound buffers.
	 */
	void unmap()
	{
#ifndef NKH_NDEBUG
	    *m_isMap = false;
#endif
	    assert(gl_object_buffer<Buff>::gl_unmap());
	    glCheck(*m_ptr = nullptr);
	}

#ifndef NKH_NDEBUG
	/**
	 * \brief Check for the pointer validity.
	 * This function make the pointer valid if it wasn't.
	 */
	void check_integrity()
	{
	    // Rude for now, but will force the code to be ok.
	    assert(*m_isMap);// && m_context
	    // Now we check for error.
	    assert(priv::glCheckError(__FILE__, __LINE__));
	}
#endif

	// ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

	template<typename, typename, typename> friend class gl_allocator;
	friend class mgl::gl_vector<T, Buff>;
	template<typename, typename> friend class gl_ptr;

	// ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

	/** The pointer to the data. */
	std::shared_ptr<pointer> m_ptr;
	/** The offset to apply to the pointer once bound. */
	difference_type          m_offset;
#ifndef NKH_NDEBUG
	/** True if the pointer is valid. */
	std::shared_ptr<bool>    m_isMap;
#endif
};

} /* namespace priv. */


template<typename Buff>
struct gl_ptr<void, Buff> : public priv::gl_ptr_impl<void, Buff>
{
    // ================================================================ //
    // ============================ TYPEDEFS ========================== //
    // ================================================================ //

    typedef priv::gl_ptr_impl<void, Buff>      base_type;
    typedef typename base_type::difference_type difference_type;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    void** operator&() { return &(*(this->m_ptr)); }

    // ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

    friend difference_type operator- <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);
    friend gl_ptr<void, Buff> operator+ <>(const gl_ptr<void, Buff>&, difference_type p_n);
    friend gl_ptr<void, Buff> operator+ <>(difference_type p_n, const gl_ptr<void, Buff>&);
    friend gl_ptr<void, Buff> operator- <>(const gl_ptr<void, Buff>&, difference_type p_n);

    friend bool operator< <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);
    friend bool operator> <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);
    friend bool operator>= <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);
    friend bool operator<= <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);

    friend bool operator== <>(const gl_ptr<void, Buff>&, const gl_ptr<void, Buff>&);
};

/**
 * \class gl_ptr is a pointer to memory located on the GPU through buffer.
 * This class behave in a slightly different way when NKH_NDEBUG isn't
 * defined. First its sizeof change as a bool is added to check correct
 * usage whenever we can. Thus the execution will be slower than without it.
 *
 * TODO : I think we should have a way to get this behavior even if
 * the Macro NKH_NDEBUG isn't defined, with for instance an other macro.
 */
template<typename T, typename Buff>
struct gl_ptr : public priv::gl_ptr_impl<T, Buff>
{
    // ================================================================ //
    // ========================= STATIC ASSERT ======================== //
    // ================================================================ //

    static_assert(std::is_standard_layout<T>::value, "The type used here must be a standard layout data type.");
    static_assert(!std::is_pointer<T>::value, "You can't use this class with pointers.");

    // ================================================================ //
    // ============================ TYPEDEFS ========================== //
    // ================================================================ //

    typedef priv::gl_ptr_impl<T, Buff>      base_type;
    typedef T &                             reference;

    typedef typename base_type::value_type      value_type;
    typedef typename base_type::difference_type difference_type;
    typedef typename base_type::pointer         pointer;
    typedef typename base_type::iterator_category
                                                iterator_category;
    typedef typename base_type::size_type       size_type;
    typedef typename base_type::void_pointer    void_pointer;
    typedef typename base_type::const_void_pointer
                                                const_void_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    using base_type::base_type;

    /**
     * \brief Copy constructor to meet allocator requirements.
     * In order to have this class being used by the gl_allocator.
     * The following must be possible :
     *  static_cast< gl_ptr<T> >(gl_ptr<void>)
     * \param vptr is a void pointer.
     */
    gl_ptr(const gl_ptr<void, Buff>& vptr)
        : base_type()
    {
        this->m_ptr = vptr.m_ptr;
        this->m_offset = vptr.m_offset;
    #ifndef NKH_NDEBUG
        this->m_isMap = vptr.m_isMap;
    #endif
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief A non void pointer is dereferencable
     * @return a reference to the data pointed by this pointer.
     */
    reference   operator*() const
    {
#ifndef NKH_NDEBUG
        if(!*(this->m_isMap))
            throw gl_buffer_not_mapped();
#endif
        return *(*(this->m_ptr)+this->m_offset);
    }

    /**
     *  Random access iterator requirements
     */
    reference   operator[](const difference_type& __n) const
    {
#ifndef NKH_NDEBUG
        if(!*(this->m_isMap))
            throw gl_buffer_not_mapped();
#endif
        return *(this->m_ptr)[this->m_offset + __n];
    }

    /**
     * @brief Taking the address of the pointer.
     * @return Return the address of the underlying pointer.
     */
    void** operator&() { return &(*(this->m_ptr)); }

    // ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

    friend difference_type operator- <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
    friend gl_ptr<T, Buff> operator+ <>(const gl_ptr<T, Buff>&, size_type p_n);
    friend gl_ptr<T, Buff> operator+ <>(const gl_ptr<T, Buff>&, difference_type p_n);
    friend gl_ptr<T, Buff> operator+ <>(difference_type p_n, const gl_ptr<T, Buff>&);
    friend gl_ptr<T, Buff> operator+ <>(const gl_ptr<T, Buff>&, size_type p_n);
    friend gl_ptr<T, Buff> operator- <>(const gl_ptr<T, Buff>&, difference_type p_n);

    friend bool operator< <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
    friend bool operator> <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
    friend bool operator>= <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
    friend bool operator<= <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);

    friend bool operator== <>(const gl_ptr<T, Buff>&, const gl_ptr<T, Buff>&);
};



/**
 * RandomAccessIterator operators requirements :
 */
    template<typename T, typename B>
    gl_ptr<T, B> operator+(const gl_ptr<T, B>& p_i, typename gl_ptr<T, B>::difference_type p_n)
    {
        return gl_ptr<T, B>(p_i.m_offset + p_n, p_i);
    }

    template<typename T, typename B>
    gl_ptr<T, B> operator+(const gl_ptr<T, B>& p_i, typename gl_ptr<T, B>::size_type p_n)
    {
        return gl_ptr<T, B>(p_i.m_offset + p_n, p_i);
    }

    template<typename T, typename B>
    gl_ptr<T, B> operator+(typename gl_ptr<T, B>::difference_type p_n, const gl_ptr<T, B>& p_i)
    {
        return p_i + p_n;
    }

    template<typename T, typename B>
    gl_ptr<T, B> operator+(typename gl_ptr<T, B>::size_type p_n, const gl_ptr<T, B>& p_i)
    {
        return p_i + p_n;
    }


//    template<typename T, typename B>
//    gl_ptr<T, B> operator-(const gl_ptr<T, B>& p_i, typename gl_ptr<T, B>::difference_type p_n)
//    {
//        return gl_ptr<T, B>(p_i.m_offset - p_n, p_i);
//    }

    template<typename T, typename B>
    typename gl_ptr<T, B>::difference_type
                operator-(const gl_ptr<T, B>& p_a, const gl_ptr<T, B>& p_b)
    {
        return p_a.m_offset - p_b.m_offset;
    }

    template<typename T, typename B>
    inline bool
    operator==( const gl_ptr<T, B>& p_a,
                const gl_ptr<T, B>& p_b)
    { return (p_a.m_ptr == p_b.m_ptr) && (p_a.m_offset == p_b.m_offset); }

    template<typename T, typename B>
    inline bool
    operator!=( const gl_ptr<T, B>& p_a,
                const gl_ptr<T, B>& p_b)
      { return !(p_a == p_b); }

    template<typename T, typename B>
    bool operator<(const gl_ptr<T, B>& p_a, const gl_ptr<T, B>& p_b)
    {
        return p_a.m_offset < p_b.m_offset;
    }

    template<typename T, typename B>
    bool operator>(const gl_ptr<T, B> & p_a, const gl_ptr<T, B>& p_b)
    {
        return p_b < p_a;
    }

    template<typename T, typename B>
    bool operator>=(const gl_ptr<T, B> & p_a, const gl_ptr<T, B>& p_b)
    {
        return !(p_a < p_b);
    }

    template<typename T, typename B>
    bool operator<=(const gl_ptr<T, B> & p_a, const gl_ptr<T, B>& p_b)
    {
        return !(p_a > p_b);
    }

} /* namespace mgl */

#endif /* GLPTR_H_ */
