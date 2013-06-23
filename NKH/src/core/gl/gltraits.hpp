/*
 * glallocator.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLALLOCATOR_HPP_
#define GLALLOCATOR_HPP_

#include <cstdint>
#include "glrequires.hpp"

namespace nkh {
namespace core {
namespace gl {

template<class T>
struct gl_buffer
{
    static constexpr GLenum target = GL_ARRAY_BUFFER;
};

template<>
struct gl_buffer<std::uint32_t>
{
    static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;
};


} /* namespace gl */
} /* namespace core */
} /* namespace nkh */


#endif /* GLALLOCATOR_HPP_ */
