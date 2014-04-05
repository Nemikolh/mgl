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

struct add_in
{
    template<typename E>
    void operator()(char* str) const
    {
        content += " in ";
        content += gl_translator<E>::type_str();
        content +=  ;
    }

    std::string content;
};

template<typename T>
char * src_pass_through_shader(shader_type p_type)
{
    add_in ins;
    mgl::for_each<T>::apply(ins);
    auto inputs = ins.content;
}


}  /* namespace priv */
}  /* namespace extension */
}  /* namespace mgl */



#endif /* DETAILS_HPP_ */
