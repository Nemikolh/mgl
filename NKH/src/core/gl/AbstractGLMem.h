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
namespace priv {

/**
 * \class AbstractGLMem
 * \brief This class
 */
class AbstractGLMem
{
public:
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    AbstractGLMem();

    /**
     *
     * \param n​
     * \param objects
     */
    //virtual void glGenMem( GLsizei n​, GLuint *objects ) = 0;

protected:
    virtual ~AbstractGLMem();
};

} /* namespace priv */
} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
#endif /* ABSTRACTGLMEM_H_ */
