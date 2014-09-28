/*
 * gpu_detail.hpp
 *
 *  Created on: 6 févr. 2014
 *      Author: nemikolh
 */

#ifndef GPU_DETAIL_HPP_
#define GPU_DETAIL_HPP_

namespace mgl {

/**
 * \class gpu_buffer
 * \brief Simple container to simplify the use of gl_ptr.
 */
template<typename T>
struct gpu_buffer
{
    typedef T * pointer;
    GLuint id;
    T *    ptr;
};

}  /* namespace mgl */

#endif /* GPU_DETAIL_HPP_ */
