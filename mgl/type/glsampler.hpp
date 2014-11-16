/*
 * glsampler.hpp
 *
 *  Created on: 15 nov. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLSAMPLER_HPP_
#define MGL_TYPE_GLSAMPLER_HPP_

#include "gltraits.hpp"
#include "priv/details.hpp"

namespace mgl {

struct gl_sampler : public priv::base_id_ref_count
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

    // ================================================================ //
    // ============================ OVERRIDE ========================== //
    // ================================================================ //

    gl_types::uid gen() override
    {
        gl_types::uid id;
        gl_object_sampler::gl_gen(1, &id);
        return id;
    }

    void gl_delete(gl_types::uid p_id) override
    {
        gl_object_sampler::gl_delete(1, &p_id);
    }
};

}  /* namespace mgl */

#endif /* MGL_TYPE_GLSAMPLER_HPP_ */
