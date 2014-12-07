/*
 * gluniform.hpp
 *
 *  Created on: 25 oct. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLUNIFORM_HPP_
#define MGL_TYPE_GLUNIFORM_HPP_

#include "../type/gltraits.hpp"

namespace mgl {

struct gl_program;

class gl_uniform {

public:
    gl_uniform()
        : m_id(-1)
    {}

    gl_types::id id() const { return m_id; }

private:
    friend gl_program;

    gl_uniform(gl_types::id p_id)
        : m_id(p_id)
    {}

    gl_types::id m_id;
};

}  /* namespace mgl */


#endif /* MGL_TYPE_GLUNIFORM_HPP_ */
