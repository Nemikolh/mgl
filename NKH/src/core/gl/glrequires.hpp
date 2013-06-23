/*
 * glrequires.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef GLREQUIRES_HPP_
#define GLREQUIRES_HPP_

#include <GL/glew.h>

namespace nkh {
namespace core {
namespace gl {

/*
 * Dirty Macro to perform check only on debug mode.
 * This code comes from the SFML GLCheck.hpp file.
 */
#ifndef NDEBUG

    // In debug mode, perform a test on every OpenGL call
    #define glCheck(call) ((call), nkh::core::gl::priv::glCheckError(__FILE__, __LINE__))

#else

    // Else, we don't add any overhead
    #define glCheck(call) (call)

#endif

// ================================================================ //
// ========================== EXCEPTIONS ========================== //
// ================================================================ //

/**
 * \brief general gl_exception.
 */
class gl_exception : public std::exception
{
public:
    gl_exception() {}
    virtual const char* what() const { return "gl_exception"; }
};

/**
 * \brief gl_out_of_memory exception.
 */
class gl_out_of_memory : public gl_exception
{
public:
    gl_out_of_memory() {}
    const char* what() const override { return "gl_out_of_memory"; }
};

/**
 * \brief gl_invalid_enum
 */
class gl_invalid_enum : public gl_exception
{
public:
    gl_invalid_enum() {}
    const char* what() const override { return "gl_invalid_enum"; }
};

/**
 * \brief gl_invalid_value
 */
class gl_invalid_value : public gl_exception
{
public:
    gl_invalid_value() {}
    const char* what() const override { return "gl_invalid_value"; }
};

/**
 * \brief gl_invalid_operation
 */
class gl_invalid_operation : public gl_exception
{
public:
    gl_invalid_operation() {}
    const char* what() const override { return "gl_invalid_operation"; }
};

/**
 * \brief gl_stack_overflow
 */
class gl_stack_overflow : public gl_exception
{
public:
    gl_stack_overflow() {}
    const char* what() const override { return "gl_stack_overflow"; }
};

/**
 * \brief gl_stack_underflow
 */
class gl_stack_underflow : public gl_exception
{
public:
    gl_stack_underflow() {}
    const char* what() const override { return "gl_stack_underflow"; }
};

// ================================================================ //
// ========================= PRIV NAMESPACE ======================= //
// ================================================================ //

namespace priv {

/**
 * \brief Check the last OpenGL error
 * \param file Source file where the call is located
 * \param line Line number of the source file where the call is located
 */
void glCheckError(const char* file, unsigned int line);

/**
 * \brief Same as glCheck but throw an exception if an error occured.
 */
void glTryError();

} /* namespace priv */


} /* namespace gl */
} /* namespace core */
} /* namespace nkh */

#endif /* GLREQUIRES_HPP_ */
