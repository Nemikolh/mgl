/*
 * default_shaders.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef DEFAULT_SHADERS_HPP_
#define DEFAULT_SHADERS_HPP_

#include "../type/glshader.hpp"
#include "priv/details.hpp"

namespace mgl {
namespace extension {

template<typename T>
gl_shader pass_through_shader(shader_type p_type)
{
    gl_shader sh(p_type);
    std::string tmp(priv::src_pass_through_shader<T>(p_type));
    sh.load_src(tmp.c_str());
    return sh;
}

}  /* namespace extension */
}  /* namespace mgl */

#endif /* DEFAULT_SHADERS_HPP_ */
