/*
 * glmemptr.h
 *
 *  Created on: 13 avr. 2013
 *      Author: nemikolh
 */

#ifndef GLMEMPTR_H_
#define GLMEMPTR_H_

#include <type_traits>
#include <GL/gl.h>
#include "glid.hpp"

namespace nkh {
namespace core {
namespace gl {

/**
 * \class glmem_ptr
 * \brief The glmem_ptr class allows to controls the data that lie on the graphic cards in an abstract way.
 *
 * This is a resource management class, designed to work in the same way as std::shared_ptr.
 * It does reference counting on the resource you want to handle. Currently, you can handle
 * with this class, only the following ones :
 *  - VertexBuffer
 *  - IndexBuffer
 *  - FrameBuffer
 * All those classes derived from AbstractGLMem
 */
template<class T>
class glmem_ptr : public T
{
public:

    // ================================================================ //
    // ========================= STATIC ASSERT ======================== //
    // ================================================================ //

    static_assert(std::is_base_of<gl_id, T>::value, "The class T must inherit of gl_id.");
    static_assert(std::is_abstract<T>::value, "The class T mustn't implement the id() method.");
    static_assert(sizeof(T) - sizeof(gl_id) == 0, "The class T must be empty regardless from the inherited gl_id");

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default Constructor.
     */
	glmem_ptr():
	    m_id{0}
    {
	    // Does nothing special
    }

	/**
	 * \brief Copy Constructor.
	 */
	glmem_ptr(glmem_ptr& rhs):
	    m_id{rhs.m_id}
	{
	    // TODO : should avert the glmem_manager than the reference count has been incremented.
	}

	/**
	 * \brief Destructor.
	 */
	~glmem_ptr()
	{
	    // TODO : decrease the reference counter.
	}

	glmem_ptr& operator= (const glmem_ptr& rhs)
	{
	    T::operator=(rhs);
	    // TODO : idem as previous. Plus implement the case of polymorphism.
	    m_id = rhs.m_id;
	    return *this;
	}

	/**
	 * \brief Pointer interface
	 * To use this object as if it was a real pointer.
	 */
	T*       operator->()
	{
	    return this;
	}

	/**
     * \brief Pointer interface
     * To use this object as if it was a real pointer.
     */
	const T* operator->() const
	{
	    return this;
	}

	/**
     * \brief Pointer interface
     * To use this object as if it was a real pointer.
     */
	T&       operator*()
	{
	    return *this;
	}

	/**
     * \brief Pointer interface
     * To use this object as if it was a real pointer.
     */
	const T& operator*() const
	{
	    return *this;
	}

private:
	/** The shared id. */
	GLuint m_id;
};

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
#endif /* GLMEMPTR_H_ */
