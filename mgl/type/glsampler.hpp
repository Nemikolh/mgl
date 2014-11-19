/*
 * glsampler.hpp
 *
 *  Created on: 15 nov. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLSAMPLER_HPP_
#define MGL_TYPE_GLSAMPLER_HPP_

#include "../glrequires.hpp"
#include "priv/details.hpp"

namespace mgl {

enum class filtering
{
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class wrap_mode
{
    REPEAT,                 //!< Equivalent of GL_REPEAT
    MIRRORED_REPEAT,        //!< Equivalent of GL_MIRRORED_REPEAT
    CLAMP_TO_EDGE​,          //!< Equivalent of GL_CLAMP_TO_EDGE
    CLAMP_TO_BORDER,        //!< Equivalent of GL_CLAMP_TO_BORDER
    MIRROR_CLAMP_TO_EDGE,   //!< Requires OpenGL 4.4 (Equivalent to GL_MIRROR_CLAMP_TO_EDGE)
};


namespace mutable_gl {

struct sampler;

}  /* namespace mutable_gl */

/**
 * @brief gl_sampler is a class allowing to manipulate sampler.
 * The gl_sampler has the following semantics:
 *
 *   @code
 *      // A sampler constructed with default ctor
 *      // does not reference an existing opengl object
 *      gl_sampler a;
 *
 *      // Generate a new sampler object and set the property.
 *      a.set_mag_filter(filtering::NEAREST);
 *
 *      // They have copy on write semantics:

 *      //  b, a and c refer to the same opengl object
 *      //  as the copy constructor and assignment operator
 *      //  have shared copy semantics.
 *      gl_sampler b(a);
 *      gl_sampler c = a;
 *
 *      //  b now refer to a different object as we have
 *      //  performed a write operation.
 *      b.set_mag_filter(filtering::LINEAR)
 *       .set_min_filter(filtering::LINEAR_MIPMAP_LINEAR);
 *

 *      // But you can still modify the shared object if you want to:
 *      mut(a).set_wrap_s(wrap_mode::REPEAT);
 *   @endcode
 *
 * Keep in mind that you have to use the chaining mechanism. It use a dedicated
 * class to control when copy should occurs. For example, the following code
 * will force many undesired deep copies to occurs.
 *
 *   @code
 *      // One copy here
 *      b.set_mag_filter(...);
 *      // and another copy here
 *      b.set_min_filter(...);
 *   @endcode
 *
 * Note:
 * -----
 *   We could have checked for the ref count and do only the copy if its value is
 *   greater than one. But it still implies having the helper object being created
 *   twice. So to avoid any overhead, use the chaining or capture the result with an
 *   auto variable.
 */
struct gl_sampler : public priv::base_id_ref_count<gl_sampler>
{
    typedef priv::base_id_ref_count base;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * Default ctor
     */
    gl_sampler() noexcept
    : base()
    {}

    /**
     * @brief Default copy ctor
     */
    gl_sampler(const gl_sampler&) = default;

    /**
     * @brief Default assignment operator
     */
    gl_sampler& operator=(const gl_sampler&) = default;

    /**
     * @brief Default move ctor
     */
    gl_sampler(gl_sampler&&) = default;

    /**
     * @brief Default assignment move operator.
     */
    gl_sampler& operator=(gl_sampler&&) = default;

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * @brief Bind the sampler to the given texture unit.
     * @param p_texture_unit is the texture unit.
     */
    void bind(GLuint p_texture_unit) const
    {
        gl_object_sampler::gl_bind(p_texture_unit, id());
    }


    // TODO if this sampler is for depth texture then
    // we must allow only texture that have a depth component
    // in their internal format

private:

    gl_types::uid gen()
    {
        gl_types::uid id;
        gl_object_sampler::gl_gen(1, &id);
        return id;
    }

    void gl_delete(gl_types::uid p_id)
    {
        gl_object_sampler::gl_delete(1, &p_id);
    }
};

namespace mutable_gl {

struct sampler
{
    sampler(const gl_sampler& p_sampler)
        : m_id(p_sampler.clone())
    {}

    sampler(gl_sampler& p_sampler)
        : m_id(p_sampler.id())
    {}

    ~sampler()
    {

    }

    /**
     * @brief Set the magnification filter for this sampler.
     * @param p_filter is the filter chosen.
     */
    sampler& set_mag_filter(filtering p_filter)
    {
        glCheck(glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, convert_to_en(p_filter)));
        return this;
    }

    /**
     * @brief Set the minification filter for this sampler.
     * Note, if used with anisotropic filtering, it is recommended to set this
     * parameter to filtering::LINEAR_MIPMAP_LINEAR.
     * @param p_filter is the filter chosen.
     */
    sampler& set_min_filter(filtering p_filter)
    {
        glCheck(glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, convert_to_en(p_filter)));
        return this;
    }

    /**
     * @brief Set the anisotropic filtering
     * According to https://www.opengl.org/wiki/Sampler_Object:
     * "For best results, combine a anisotropic filtering with a LINEAR_MIPMAP_LINEAR
     * ​ minification filter."
     * @param p_value is the
     */
    sampler& set_anisotropic_filtering(float p_value)
    {
#       ifndef MGL_NDEBUG
        GLfloat largest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
        assert(p_value >= 1.0f && p_value <= largest);
#       endif
        glCheck(glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, p_value));
        return this;
    }

    /**
     * @brief Set the maximum value for the anisotropic filtering.
     * Also set the minification filter to the appropriate value in the same time.
     */
    sampler& set_max_anisotropic_filtering()
    {
        GLfloat largest_value;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_value);
        glCheck(glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_value));
        return this;
    }

    template<wrap_mode Mode>
    sampler& set_wrap_s()
    {
        glCheck(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, convert_to_en(Mode)));
        return this;
    }


private:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    inline GLenum convert_to_en(filtering p_tag) const
    {
        switch (p_tag) {
            case filtering::LINEAR:
            return GL_LINEAR;
            case filtering::NEAREST:
            return GL_NEAREST;
            case filtering::LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
            case filtering::NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
            case filtering::NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
            case filtering::LINEAR_MIPMAP_LINEAR:
            default:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
    }

    inline GLenum convert_to_en(wrap_mode p_mode) const
    {
        switch (p_mode) {
            case wrap_mode::MIRRORED_REPEAT:
                return GL_MIRRORED_REPEAT;
            case wrap_mode::MIRROR_CLAMP_TO_EDGE:
                return GL_MIRROR_CLAMP_TO_EDGE;
            case wrap_mode::CLAMP_TO_EDGE​:
                return GL_CLAMP_TO_EDGE;
            case wrap_mode::CLAMP_TO_BORDER:
                return GL_CLAMP_TO_BORDER;
            case wrap_mode::REPEAT:
            default:
                return GL_REPEAT;
        }
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    const&
};

} /* namespace mutable_gl */

} /* namespace mgl */

#endif /* MGL_TYPE_GLSAMPLER_HPP_ */
