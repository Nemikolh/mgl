/*
 * default_shaders.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef DEFAULT_SHADERS_HPP_
#define DEFAULT_SHADERS_HPP_

#include "../type/glshader.hpp"
#include "priv/genheader.hpp"
#include "../meta/gliterdata.hpp"

namespace mgl {
namespace extension {

namespace priv {
template<typename T>
std::string src_pass_through_shader(shader_type p_type)
{
    gen_header ins;
    gen_header outs;
    std::string body;
    auto ignore_first = [](std::size_t N) { return N > 0; };
    switch(p_type)
    {
        case shader_type::VERTEX_SHADER:
            ins.set_qualifier("in");
            outs.set_qualifier("smooth out");
            outs.set_prefix("frag_in_");
            outs.set_test(ignore_first);
            body = "void main(void){\n gl_Position = vec4(position, 1.0);\n frag_in_color = color;\n}";
            break;
        case shader_type::FRAGMENT_SHADER:
            ins.set_qualifier("smooth in");
            ins.set_prefix("frag_in_");
            ins.set_test(ignore_first);
            outs.set_qualifier("out");
            outs.set_prefix("frag_out_");
            outs.set_test(ignore_first);
            body = "void main(void){\n frag_out_color = frag_in_color;\n}";
            break;
        default:
            return "";
    }
    mgl::for_each<T>::apply(ins);
    mgl::for_each<T>::apply(outs);
    auto inputs = "#version 330\n" + ins.content + outs.content + body;
    return inputs;
}
} /* namespace priv. */

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
