/*
 * AbstractGLMem.h
 *
 *  Created on: 17 avr. 2013
 *      Author: nemikolh
 */

#ifndef ABSTRACTGLMEM_H_
#define ABSTRACTGLMEM_H_

#include "../../external/external_gl.hpp"

namespace nkh {
namespace core {
namespace gl {

/**
 * \class Glid.
 * \brief All class managing OpenGL object must inherit from this class.
 */
class gl_id
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    /**
     * \brief Default constructor.
     */
    gl_id();

    /**
     * \brief Destructor.
     */
    virtual ~gl_id() = 0;

protected:

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    /**
     * \brief This function can be used to retrieved the id.
     */
    virtual GLuint id() = 0;
};

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
#endif /* ABSTRACTGLMEM_H_ */
