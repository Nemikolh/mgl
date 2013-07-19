/*
 * glshadercontext.hpp
 *
 *  Created on: 19 juil. 2013
 *      Author: nemikolh
 */

#ifndef GLSHADERCONTEXT_HPP_
#define GLSHADERCONTEXT_HPP_

namespace nkh {
namespace core {
namespace gl {


/**
 * \brief gl_shader_context TODO define it properly.
 * \param M is the material type.
 */
template<typename M>
class gl_shader_context
{
};


// Used for test purposes :
// ------------------------
//
//    template<>
//    struct gl_shader_context<int>
//    {
//        void bind_attribute(
//                std::string name,                   // attribute name
//                int component_number,               // number of component
//                std::size_t byte_offset,            // offsetof(Seq, name_t)
//                std::size_t stride,                 // stride
//                GLenum type                         // deduce
//            )
//        const
//        {
//            using namespace std;
//            cout << name << " with " << component_number << " components";
//            cout << ", an offset of : " << byte_offset << " in a stride of " << stride;
//            cout << " for the GLenum " << type << endl;
//        }
//    };
//


} /* namespace gl */
} /* namespace core */
} /* namespace nkh */


#endif /* GLSHADERCONTEXT_HPP_ */
