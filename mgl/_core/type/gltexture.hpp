/*
 * gltexture.hpp
 *
 *  Created on: 15 nov. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLTEXTURE_HPP_
#define MGL_TYPE_GLTEXTURE_HPP_

#include "gltraits.hpp"
#include "priv/details.hpp"
#include "gltexturetype.hpp"

namespace mgl {

/*
 * Forward declaration of gl_texture.
 */
template<typename Kind>
struct gl_texture;

/**
 * @brief gl_texture_1D type definition.
 */
using gl_texture_1D = gl_texture<priv::texture_type_1D>;

/**
 * @brief gl_texture_buffer type definition.
 */
using gl_texture_buffer = gl_texture<priv::texture_type_buffer>;

/**
 * @brief gl_texture_2D type definition
 */
using gl_texture_2D = gl_texture<priv::texture_type_2D>;

/**
 * @brief
 */


/*
 * Implementation details
 */
template<typename Kind>
struct gl_texture : public priv::base_id_ref_count
{
    typedef priv::base_id_ref_count base;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * @brief Default ctor.
     */
    gl_texture() noexcept
        : base()
    {}

    /**
     * @brief Default copy ctor.
     */
    gl_texture(const gl_texture&) = default;

    /**
     * @brief Default assignment operator
     */
    gl_texture& operator=(const gl_texture&) = default;

    ~gl_texture()
    {
        release();
    }

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

private:

    // ================================================================ //
    // ============================ OVERRIDE ========================== //
    // ================================================================ //

    gl_types::uid gen() override
    {
        gl_types::uid id;
        gl_object_texture::gl_gen(1, &id);
        return id;
    }

    void gl_delete(gl_types::uid p_id) override
    {
        gl_object_texture::gl_delete(1, &p_id);
    }
};

}  /* namespace mgl */

#endif /* MGL_TYPE_GLTEXTURE_HPP_ */
