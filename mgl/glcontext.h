/*
 * glcontext.h
 *
 *  Created on: 28 juin 2013
 *      Author: nemikolh
 */

#ifndef GLCONTEXT_H_
#define GLCONTEXT_H_

namespace nkh {
namespace core {
namespace gl {

class gl_format;

namespace priv {
class gl_context_impl;
}

/**
 * \class gl_context is the representation of a gl context.
 */
class gl_context
{
public:

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Construct an OpenGL context with the passed format.
     * \param p_format is the format for this context.
     */
    gl_context(const gl_format & p_format);

    /**
     * \brief Destructor.
     */
    ~gl_context();

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief Bind the passed buffer.
     * \param p_target is the target of the buffer.
     * \param p_buffer is the  buffer id.
     */
    template<GLenum target>
    void bind(GLuint p_buffer)
    {
        m_impl->bind(target, p_buffer);
    }

    /**
     * \brief Unbind the passed buffer.
     * \param p_target is the target of the buffer.
     * \param p_buffer is the buffer id.
     */
    template<GLenum target>
    void unbind(GLuint p_id)
    {
        m_impl->unbind(target);
    }

    template<typename ObjType>
    void decrement_count(GLuint p_id)
    {
        m_impl->decrement_count<ObjType>(p_id);
    }

    template<typename ObjType>
    void increment_count(GLuint p_id)
    {
        m_impl->increment_count<ObjType>(p_id);
    }

    /**
     * \brief Create a buffer.
     * @param p_oldValue
     * @return
     */
    GLuint create(GLuint p_oldValue)
    {

    }

    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    /**
     * \brief To test if two gl_context are sharing resources.
     * \param p_first is a gl_context.
     * \param p_second is an other gl_context.
     * \return True if they are sharing resources.
     */
    static bool are_sharing_resources(const gl_context & p_first, const gl_context & p_second);

    /**
     * \brief Returns the currently bound context.
     * This function return a pointer as no assumption should be made
     * \return
     */
    static gl_context * current_context();j'aime pas ce code du tout... Virer ce pointeur de merde/Faire quelque chose Putain.

private:
    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    /** Pimpl idiom / D-pointer. */
    priv::gl_context_impl * m_impl;
};


} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
#endif /* GLCONTEXT_H_ */
