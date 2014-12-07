/*
 * genheader.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: nemikolh
 */

#ifndef EXTENSION_PRIV_DETAILS_GEN_HEADER_HPP_
#define EXTENSION_PRIV_DETAILS_GEN_HEADER_HPP_

#include <string>
#include "../../shader/glsltranslator.hpp"

namespace mgl {
namespace extension {
namespace priv {

struct gen_header
{
    gen_header()
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
            content += glsl_translator<E>::type_str();
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

}  /* namespace priv */
}  /* namespace extension */
}  /* namespace mgl */



#endif /* EXTENSION_PRIV_DETAILS_GEN_HEADER_HPP_ */
