/*
 * glptr.h
 *
 *  Created on: 13 avr. 2013
 *      Author: nemikolh
 */

#ifndef GLPTR_H_
#define GLPTR_H_

#include <type_traits>
#include <GL/glew.h>
#include <assert.h>

namespace nkh {
namespace core {
namespace gl {

/**
 * \class gl_ptr is a pointer to memory located on the GPU through buffer.
 * This class behave in a slightly different way when NKH_NDEBUG isn't
 * defined. First its sizeof change as a bool is added to check correct
 * usage whenever we can. Thus the execution will be slower than without it.
 *
 * TODO : I think we should have a way to get this behavior even if
 * the Macro NKH_NDEBUG isn't defined, with for instance an other macro.
 */
template<class T, typename Buff = gl_buffer<T> >
class gl_ptr
{
public:

    // ================================================================ //
    // ========================= STATIC ASSERT ======================== //
    // ================================================================ //

    static_assert(std::is_standard_layout<T>::value, "The type used here must be a standard layout data type.");
    static_assert(!std::is_pointer<T>::value, "You can't use this class with pointers.");

    // ================================================================ //
    // ============================ TYPEDEF =========================== //
    // ================================================================ //

    typedef size_t                          size_type;
    typedef T                               value_type;
    typedef ptrdiff_t                       difference_type;
    typedef T &                             reference;
    typedef T *                             pointer;
    typedef std::random_access_iterator_tag iterator_category;

    typedef gl_context *                    context;
    typedef gl_ptr<void>                    void_pointer;
    typedef const gl_ptr<void>              const_void_pointer;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default Constructor.
     */
	gl_ptr(std::nullptr_t = nullptr)
	    : m_id{0}
	    , m_ptr{nullptr}
	    , m_offset{0}
        #ifndef NKH_NDEBUG
	    , m_isMap{false}
        #endif
	    , m_context{nullptr}
    {}

    /**
     * \brief Copy constructor
     * \param rhs is an other gl_ptr.
     */
    gl_ptr(const gl_ptr& rhs)
        : m_id{rhs.m_id}
        , m_ptr{rhs.m_ptr}
        , m_offset{rhs.m_offset}
        #ifndef NKH_NDEBUG
        , m_isMap{rhs.m_isMap}
        #endif
        , m_context{rhs.m_context}
    {}

	/**
	 * \brief Copy Constructor.
	 * \param rhs is a gl_ptr where U is convertible into T.
	 */
    template<typename U, typename B, typename = typename
            std::enable_if<std::is_convertible<U*, T*>::value>::type>
	gl_ptr(const gl_ptr<U, B>& rhs)
	    : m_id{rhs.m_id}
	    , m_ptr{rhs.m_ptr}  // why not static_cast? -> to avoid accepting downcasting. Actually is_convertible wouldn't accept it.
	    , m_offset{rhs.m_offset}
        #ifndef NKH_NDEBUG
        , m_isMap{rhs.m_isMap}
        #endif
        , m_context{rhs.m_context}
	{}

	/**
	 * \brief Copy constructor to meet allocator requirements.
	 * In order to have this class being used by the gl_allocator.
	 * The following must be possible :
	 *  static_cast< gl_ptr<T> >(gl_ptr<void>)
	 * \param vptr is a void pointer.
	 */
	gl_ptr(const gl_ptr<void>& vptr) // eclipse CDT n'est pas à l'aise avec C++11
	    : gl_ptr()
	{
	}

	/**
	 * \brief Move constructor.
	 * \param rhs the moved value.
	 */
	gl_ptr(gl_ptr && rhs)
        : m_id{std::move(rhs.m_id)}
        , m_ptr{std::move(rhs.m_ptr)}
        , m_offset{std::move(rhs.m_offset)}
        #ifndef NKH_NDEBUG
        , m_isMap{std::move(rhs.m_isMap)}
        #endif
        , m_context{std::move(rhs.m_context)}
	{}

	/**
     * \brief Move constructor.
     * \param rhs the moved value.
     */
    template<typename U, typename B, typename = typename
            std::enable_if<std::is_convertible<U*, T*>::value>::type>
    gl_ptr(gl_ptr<U, B>&& rhs)
        : m_id{std::move(rhs.m_id)}
        , m_ptr{std::move(rhs.m_ptr)}
        , m_offset{std::move(rhs.m_offset)}
        #ifndef NKH_NDEBUG
        , m_isMap{std::move(rhs.m_isMap)}
        #endif
        , m_context{std::move(rhs.m_context)}
    {}

	/**
	 * \brief Destructor.
	 */
	~gl_ptr()
	{
	}

	gl_ptr& operator= (const gl_ptr& rhs)
	{
	    m_id = rhs.m_id;
	    m_ptr = rhs.m_ptr;
	    m_offset = rhs.m_offset;
#ifndef NKH_NDEBUG
	    m_isMap = rhs.m_isMap;
#endif
	    return *this;
	}

	gl_ptr& operator= (std::nullptr_t)
	{
	    m_id = 0;
	    m_ptr = nullptr;
	    m_offset = 0;
#ifndef NKH_NDEBUG
        m_isMap = false;
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
        if(!m_isMap)
            throw gl_exception("Pointer access not mapped");
#endif
	    return (m_ptr+m_offset);
	}

	reference   operator*() const
	{
#ifndef NKH_NDEBUG
        if(!m_isMap)
            throw gl_exception("Pointer access not mapped");
#endif
	    return *(m_ptr+m_offset);
	}

	gl_ptr&     operator++()
    {
	    ++m_offset;
	    return *this;
    }

	gl_ptr      operator++(int)
    {
	    return gl_ptr(m_offset++, *this);
    }

    /**
     *  Bidirectional iterator requirements
     */
	gl_ptr&     operator--()
    {
	    --m_offset;
	    return *this;
    }

	gl_ptr      operator--(int)
    {
	    return gl_ptr(m_offset--, *this);
    }

    /**
     *  Random access iterator requirements
     */
    reference   operator[](const difference_type& __n) const
    {
#ifndef NKH_NDEBUG
        if(!m_isMap)
            throw gl_exception("Pointer access not mapped");
#endif
        return m_ptr[m_offset + __n];
    }

    gl_ptr&     operator+=(const difference_type& p_n)
    {
        m_offset += p_n; return *this;
    }

    gl_ptr      operator+(const difference_type& p_n) const
    {
        return gl_ptr(m_offset + p_n, *this);
    }

    gl_ptr&     operator-=(const difference_type& p_n)
    {
        m_offset -= p_n; return *this;
    }

    gl_ptr      operator-(const difference_type& p_n) const
    {
        return gl_ptr(m_offset - p_n, *this);
    }

    /**
     * \brief Return true if the pointer is mapped and allocated.
     * Note : This is only true when NKH_NDEBUG isn't defined.
     * This can lead to weird behavior when this macro isn't define
     * if you have defined the NDEBUG macro.
     * \return true if the pointer is non null.
     */
    operator bool() const
    {
#ifndef NKH_NDEBUG
        assert(m_isMap);
        return m_isMap && m_ptr != nullptr;
#else
        return m_ptr != nullptr;
#endif
    }

    /**
     * \brief Set the context for this pointer.
     * \param p_context is the context.
     */
    void set_context(context p_context)
    {
        m_context = p_context;
    }

private:

    gl_ptr(difference_type p_offset, const gl_ptr & p_other)
        : m_id{p_other.m_id}
        , m_ptr{p_other.m_ptr}
        , m_offset{p_offset}
#ifndef NKH_NDEBUG
        , m_isMap{p_other.m_isMap}
#endif
        , m_context{p_other.m_context}
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
	    m_isMap = true;
#endif
	    // We make the assumption than the buffer content isn't used in draw call, that's why we have the GL_MAP_UNSYNCHRONIZED_BIT flag
	    // And finally, because of the static_assert, the cast can't fail.
	    m_ptr = reinterpret_cast<T*>(
	            gl_object_buffer::gl_map_range(Buff::target,
	                                           p_offset * sizeof(T),
	                                           p_length * sizeof(T),
	                                           GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
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
	    m_isMap = false;
#endif
//	    glCheck(glUnmapBuffer(Buff::target));
	    gl_object_buffer::gl_unmap(Buff::target);
	    m_ptr = nullptr;
	}

	/**
	 * \brief Bind the underlying buffer to Buff::target point.
	 */
	void bind()
	{
	    gl_object_buffer::gl_bind(m_id);
	}

	/**
	 * \brief Create a valid buffer id for this, with Buff::target.
	 */
	void create()
	{
	    gl_object_buffer::gl_gen(1, &m_id);
	}

#ifndef NKH_NDEBUG
	/**
	 * \brief Check for the pointer validity.
	 * This function make the pointer valid if it wasn't.
	 */
	void check_integrity()
	{
	    // Rude for now, but will force the code to be ok.
	    assert(m_isMap && m_context);
	    // Now we check for error.
	    priv::glTryError();
	}
#endif

	// ================================================================ //
    // ============================ FRIENDS =========================== //
    // ================================================================ //

	friend gl_allocator<T, Buff>;
	template<typename U, typename B> friend class gl_ptr<U, B>;

	template<typename U, typename B> friend gl_ptr<U, B> operator+(const gl_ptr<U, B>&, difference_type p_n);
	template<typename U, typename B> friend gl_ptr<U, B> operator+(difference_type p_n, const gl_ptr<U, B>&);
	template<typename U, typename B> friend gl_ptr<U, B> operator-(const gl_ptr<U, B>&, difference_type p_n);

	template<typename U, typename B> friend difference_type operator-(const gl_ptr<U, B>&, const gl_ptr<U, B>&);

	template<typename U, typename B> friend bool operator<(const gl_ptr<U, B>&, const gl_ptr<U, B>&);
	template<typename U, typename B> friend bool operator>(const gl_ptr<U, B>&, const gl_ptr<U, B>&);

	template<typename U, typename B> friend bool operator>=(const gl_ptr<U, B>&, const gl_ptr<U, B>&);
	template<typename U, typename B> friend bool operator<=(const gl_ptr<U, B>&, const gl_ptr<U, B>&);

	// ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

	/** The shared id. */
	GLuint          m_id;
	/** The pointer to the data. */
	pointer         m_ptr;
	/** The offset to apply to the pointer once bound. */
	difference_type m_offset;
#ifndef NKH_NDEBUG
	/** True if the pointer is valid. */
	bool            m_isMap;
#endif
	/** The gl context associated with this pointer. */
	context         m_context;
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
    gl_ptr<T, B> operator+(typename gl_ptr<T, B>::difference_type p_n, const gl_ptr<T, B>& p_i)
    {
        return p_i + p_n;
    }

    template<typename T, typename B>
    gl_ptr<T, B> operator-(const gl_ptr<T, B>& p_i, typename gl_ptr<T, B>::difference_type p_n)
    {
        return gl_ptr<T, B>(p_i.m_offset - p_n, p_i);
    }

    template<typename T, typename B>
    typename gl_ptr<T, B>::difference_type
                operator-(const gl_ptr<T, B>& p_a, const gl_ptr<T, B> p_b)
    {
#       ifndef NKH_NDEBUG
            assert(p_a.m_id == p_b.m_id);
#       endif
        return p_a.m_offset - p_b.m_offset;
    }

    template<typename T, typename B>
    bool operator<(const gl_ptr<T, B>& p_a, const gl_ptr<T, B>& p_b)
    {
#       ifndef NKH_NDEBUG
            assert(p_a.m_id == p_b.m_id);
#       endif
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

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */

#endif /* GLPTR_H_ */
