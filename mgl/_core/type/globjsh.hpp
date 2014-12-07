/*
 * globjsh.hpp
 *
 *  Created on: 16 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLOBJSH_HPP_
#define GLOBJSH_HPP_

#include <string>

namespace mgl {

struct gl_object_program
{
    /**
     * @brief Create an openGL program.
     * @return Return the id of the newly created program.
     */
    static inline GLuint gl_gen()
    {
        return glCreateProgram();
    }

    /**
     * @brief Deletes the passed openGL programs.
     * @param p_n is the number of ids.
     * @param p_buffers is the array of ids.
     */
    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        for(int i = 0; i < p_n; ++i)
            glDeleteProgram(p_buffers[i]);
    }

    /**
     * @brief Call glUseProgram on the passed program.
     * @param p_id is the id of the program.
     */
    static inline void gl_use(GLuint p_id)
    {
        glCheck(glUseProgram(p_id));
    }

    /**
     * @brief Attach the passed shader to the program.
     * @param p_program_id is the id of the program.
     * @param p_shader_id is the id of the shader.
     */
    static inline void gl_attach_shader(GLuint p_program_id, GLuint p_shader_id)
    {
        glCheck(glAttachShader(p_program_id, p_shader_id));
    }

    static inline void gl_detach_shader(GLuint p_program_id, GLuint p_shader_id)
    {
        glCheck(glDetachShader(p_program_id, p_shader_id));
    }

    /**
     * @brief Link the passed program.
     * @param p_program_id is the id of the program to link to.
     */
    static inline void gl_link(GLuint p_program_id)
    {
        glCheck(glLinkProgram(p_program_id));
    }

    static inline GLint gl_uniform_location(GLuint p_program_id, const char * p_name)
    {
        GLint loc;
        glCheck(loc = glGetUniformLocation(p_program_id, p_name));
        return loc;
    }

    /**
     * @brief Test the link status of the specified program.
     * @param p_program_id is the id of the program.
     * @return Return true if link was done and successful.
     */
    static inline bool gl_link_status(GLuint p_program_id)
    {
        GLint test;
        glGetProgramiv(p_program_id, GL_LINK_STATUS, &test);
        return test == GL_TRUE;
    }

    static inline std::string gl_info_log(GLuint p_id)
    {
        std::string log;
        GLint length;
        // ------------------------- DECLARE ------------------------ //

        // Retrieve the length of the log.
        glGetProgramiv(p_id, GL_INFO_LOG_LENGTH, &length);
        if(length > 0)
        {
            // runtime-sized array available in C++1y
            char log_data[length]; // Clang does not complain, GCC compiles fine but with a warning.
            glCheck(glGetProgramInfoLog(p_id, length, nullptr, log_data));
            log = std::string(log_data);
        }

        return log;
    }
};

struct gl_object_shader
{
    /**
     * \brief Generate an id for ShaderType.
     * \return Returns the generated id.
     */
    static inline GLuint gl_gen(GLenum p_shader_type)
    {
        return glCreateShader(p_shader_type);
    }

    /**
     * @brief Delete the passed buffers names.
     * @param p_n is the number of buffers.
     * @param p_buffers is the array of buffers.
     */
    static inline void gl_delete(GLsizei p_n, const GLuint * p_buffers)
    {
        for(int i = 0; i < p_n; ++i)
            glDeleteShader(p_buffers[i]);
    }

    /**
     * @brief Compile the passed shader source.
     * @param p_id is the id of the shader.
     * @param p_count is the number of elements in the p_string​ and p_length​ arrays.
     * @param p_string is an array of pointers to strings containing the source code to be loaded into the shader.
     * @param p_length is the array of string length for p_string.
     */
    static inline void gl_shader_source(GLuint p_id, GLsizei p_count, const GLchar ** p_string, const GLint * p_length)
    {
        glCheck(glShaderSource(p_id, p_count, p_string, p_length));
    }

    /**
     * @brief Compile the specified shader.
     * @param p_id is the id of the shader.
     */
    static inline void gl_compile(GLuint p_id)
    {
        glCheck(glCompileShader(p_id));
    }

    /**
     * @brief Test the compile status of the specified shader.
     * @param p_id is the shader's id.
     * @return Returns true if compilation was done and successful.
     */
    static inline bool gl_compile_status(GLuint p_id)
    {
        GLint test;
        glGetShaderiv(p_id, GL_COMPILE_STATUS, &test);
        return test == GL_TRUE;
    }

    /**
     * @brief Retrieve the log info for the compilation step.
     * @param p_id is the id of the shader.
     * @return Returns a string containing the log info.
     */
    static inline std::string gl_info_log(GLuint p_id)
    {
        std::string log;
        GLint length;
        // ------------------------- DECLARE ------------------------ //

        // Retrieve the length of the log.
        glGetShaderiv(p_id, GL_INFO_LOG_LENGTH, &length);
        if(length > 0)
        {
            // runtime-sized array available in C++1y
            char log_data[length]; // Clang does not complain, GCC compiles fine but with a warning.
            glCheck(glGetShaderInfoLog(p_id, length, nullptr, log_data));
            log = std::string(log_data);
        }

        return log;
    }
};


}  /* namespace mgl */



#endif /* GLOBJSH_HPP_ */
