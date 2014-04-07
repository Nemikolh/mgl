/*
 * details.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef DETAILS_HPP_
#define DETAILS_HPP_

#include <string>
#include "../../meta/gliterdata.hpp"
#include "../../shader/gltranslator.hpp"

namespace mgl {
namespace extension {
namespace priv {

struct gen_attributes
{
    gen_attributes()
        : content()
        , m_qualifier()
        , m_prefix(" ")
        , m_is_entry_acceptable([](std::size_t){ return true; })
    {}

    void set_qualifier(const char* p_qualifier)
    {
        m_qualifier = p_qualifier;
    }

    void set_prefix(const char* p_prefix)
    {
        m_prefix = " ";
        m_prefix += p_prefix;
    }

    template<typename Func>
    void set_test(Func&& p_is_entry_acceptable)
    {
        m_is_entry_acceptable = p_is_entry_acceptable;
    }

    template<typename E, std::size_t N>
    void apply(const char* str)
    {
        if(m_is_entry_acceptable(N))
        {
            content += m_qualifier + " ";
            content += gl_translator<E>::type_str();
            content += m_prefix;
            content += str;
            content += ";\n";
        }
    }

    std::string content;
private:
    std::string m_qualifier;
    std::string m_prefix;
    std::function<bool(std::size_t)> m_is_entry_acceptable;
};

template<typename T>
const char * src_pass_through_shader(shader_type p_type)
{
    gen_attributes ins;
    gen_attributes outs;
    std::string body;
    auto ignore_first = [](std::size_t N) { return N > 0; };
    switch(p_type)
    {
        case shader_type::VERTEX_SHADER:
            ins.set_qualifier("in");
            outs.set_qualifier("smooth out");
            outs.set_prefix("frag_in_");
            outs.set_test(ignore_first);
            body = "void main{\n gl_Position = position;\n frag_in_color = color;\n}";
            break;
        case shader_type::FRAGMENT_SHADER:
            ins.set_qualifier("smooth in");
            ins.set_prefix("frag_in_");
            ins.set_test(ignore_first);
            outs.set_qualifier("out");
            outs.set_prefix("frag_out_");
            outs.set_test(ignore_first);
            body = "void main{\n frag_out_color = frag_in_color;\n}";
            break;
        default:
            return "";
    }
    mgl::for_each<T>::apply(ins);
    mgl::for_each<T>::apply(outs);
    auto inputs = "#version 330\n" + ins.content + outs.content + body;
    return inputs.c_str();
}


}  /* namespace priv */
}  /* namespace extension */
}  /* namespace mgl */



#endif /* DETAILS_HPP_ */
