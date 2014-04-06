/*
 * glexceptions.hpp
 *
 *  Created on: 20 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLEXCEPTIONS_HPP_
#define GLEXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace mgl {

/**
 * \brief general gl_exception.
 */
class gl_exception : public std::exception
{
public:
    gl_exception()
    {}

    const char* what() const noexcept override
    { return "gl_exception"; }
};

/**
 * \brief gl_out_of_memory exception.
 */
class gl_out_of_memory : public gl_exception
{
public:
    gl_out_of_memory() {}
    const char* what() const noexcept override { return "gl_out_of_memory"; }
};

/**
 * \brief gl_invalid_enum
 */
class gl_invalid_enum : public gl_exception
{
public:
    gl_invalid_enum() {}
    const char* what() const noexcept override { return "gl_invalid_enum"; }
};

/**
 * \brief gl_invalid_value
 */
class gl_invalid_value : public gl_exception
{
public:
    gl_invalid_value() {}
    const char* what() const noexcept override { return "gl_invalid_value"; }
};

/**
 * \brief gl_invalid_operation
 */
class gl_invalid_operation : public gl_exception
{
public:
    gl_invalid_operation() {}
    const char* what() const noexcept override { return "gl_invalid_operation"; }
};

/**
 * \brief gl_stack_overflow
 */
class gl_stack_overflow : public gl_exception
{
public:
    gl_stack_overflow() {}
    const char* what() const noexcept override { return "gl_stack_overflow"; }
};

/**
 * @brief gl_uninitialized_buffer
 */
class gl_uninitialized_buffer : public gl_exception
{
public:
    gl_uninitialized_buffer() {}
    const char* what() const noexcept override
    {
        return "gl_uninitialized_buffer: You can't call bind on an uninitialized buffer.";
    }
};

/**
 * \brief gl_stack_underflow
 */
class gl_stack_underflow : public gl_exception
{
public:
    gl_stack_underflow() {}
    const char* what() const noexcept override { return "gl_stack_underflow"; }
};

/**
 * \brief Exception thrown when the current bound gl_context isn't valid.
 */
class gl_context_exception : public gl_exception
{
public:
    gl_context_exception() {}
    const char* what() const noexcept override { return "gl_context_exception"; }
};

/**
 * @brief Exception thrown when a buffer isn't mapped
 * and we attempted to perform an read/write operation on it.
 */
class gl_buffer_not_mapped : public gl_exception
{
public:
    gl_buffer_not_mapped() {}
    const char* what() const noexcept override { return "gl_buffer_not_mapped"; }
};

class gl_exception_specific : public gl_exception
{
public:
    gl_exception_specific(const char* p_msg, const char* p_what)
        : what_()
    {
        what_ = p_msg;
        what_ += p_what;
    }

    gl_exception_specific(std::string p_what)
        : gl_exception_specific(p_what.c_str())
    {}

    const char* what () const noexcept override { return what_.c_str(); }

private:
    std::string what_;
};

/**
 * @brief Exception thrown when a compile error has occured for a shader.
 */
class gl_compile_error : public gl_exception_specific
{
public:
    gl_compile_error(const char* p_what)
        : gl_exception_specific("compilation failed at line ", p_what)
    {}

    gl_compile_error(std::string p_what)
        : gl_compile_error(p_what.c_str())
    {}

};

class gl_link_error : public gl_exception_specific
{
public:
    gl_link_error(const char* p_what)
        : gl_exception_specific("link failed at line ", p_what)
    {}

    gl_link_error(std::string p_what)
        : gl_link_error(p_what.c_str())
    {}
};

}  /* namespace mgl */

#endif /* GLEXCEPTIONS_HPP_ */
