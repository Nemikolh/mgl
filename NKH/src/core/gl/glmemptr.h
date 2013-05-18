/*
 * glmemptr.h
 *
 *  Created on: 13 avr. 2013
 *      Author: nemikolh
 */

#ifndef GLMEMPTR_H_
#define GLMEMPTR_H_

namespace nkh {
namespace core {
namespace gl {

/**
 * \class glmem_ptr
 * \brief The glmem_ptr class allows to controls the data that lie on the graphic cards in an abstract way.
 *
 * This is a resource management class, designed to work in the same way as std::shared_ptr.
 * It does reference counting on the resource you want to handle. Currently, you can handle
 * with this class, only the following ones :
 *  - VertexBuffer
 *  - IndexBuffer
 *  - FrameBuffer
 * All those classes derived from AbstractGLMem
 */
class glmem_ptr
{
public:
	glmem_ptr();
	virtual ~glmem_ptr();
};

} /* namespace gl */
} /* namespace core */
} /* namespace nkh */
#endif /* GLMEMPTR_H_ */
