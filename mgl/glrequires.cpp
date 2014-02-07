/*
 * glrequires.cpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */


#include "glrequires.hpp"

#include <iostream>
#include <string>

namespace mgl {

namespace priv {

void glTryError()
{
    // Get the last error
    GLenum errorCode = glGetError();

    if (errorCode != GL_NO_ERROR)
    {
        // Translate into exception the error code.
        switch (errorCode)
        {
            case GL_INVALID_ENUM :      throw gl_invalid_enum();
            case GL_INVALID_VALUE :     throw gl_invalid_value();
            case GL_INVALID_OPERATION : throw gl_invalid_operation();
            case GL_STACK_OVERFLOW :    throw gl_stack_overflow();
            case GL_STACK_UNDERFLOW :   throw gl_stack_underflow();
            case GL_OUT_OF_MEMORY :     throw gl_out_of_memory();
            //case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :
            //                            throw gl_invalid_framebuffer_operator
            default:                    throw gl_exception();
        }
    }
}

bool glCheckError(const char* file, unsigned int line)
{
    // Get the last error
    GLenum errorCode = glGetError();

    if (errorCode != GL_NO_ERROR)
    {
        std::string fileString(file);
        std::string error = "unknown error";
        std::string description  = "no description";

        // Decode the error code
        switch (errorCode)
        {
            case GL_INVALID_ENUM :
            {
                error = "GL_INVALID_ENUM";
                description = "an unacceptable value has been specified for an enumerated argument";
                break;
            }

            case GL_INVALID_VALUE :
            {
                error = "GL_INVALID_VALUE";
                description = "a numeric argument is out of range";
                break;
            }

            case GL_INVALID_OPERATION :
            {
                error = "GL_INVALID_OPERATION";
                description = "the specified operation is not allowed in the current state";
                break;
            }

            case GL_STACK_OVERFLOW :
            {
                error = "GL_STACK_OVERFLOW";
                description = "this command would cause a stack overflow";
                break;
            }

            case GL_STACK_UNDERFLOW :
            {
                error = "GL_STACK_UNDERFLOW";
                description = "this command would cause a stack underflow";
                break;
            }

            case GL_OUT_OF_MEMORY :
            {
                error = "GL_OUT_OF_MEMORY";
                description = "there is not enough memory left to execute the command";
                break;
            }

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :
            {
                error = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
                description = "the object bound to FRAMEBUFFER_BINDING_EXT is not \"framebuffer complete\"";
                break;
            }
        }

        // Log the error
        std::cerr   << "An internal OpenGL call failed in "
                    << fileString.substr(fileString.find_last_of("\\/") + 1) << " (" << line << ") : "
                    << error << ", " << description
                    << std::endl;

        return false;
    }

    return true;
}

} /* namespace priv */


} /* namespace mgl */
