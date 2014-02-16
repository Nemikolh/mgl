/*
 * glenum.hpp
 *
 *  Created on: 16 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLENUM_HPP_
#define GLENUM_HPP_

namespace mgl {

/**
 * \class gl_enum_from_type defines enum types accordingly to C++ types.
 */
template<typename T>
struct gl_enum_from_type;

/**
 * \class tuple_component_type is a MetaFunction to specify the gl value type of your tuple.
 *
 */
template<typename T>
struct tuple_component_type
{
    static constexpr GLenum value = gl_enum_from_type<typename T::value_type>::value;
};

/**
 * Partial specialization for C++ primitives types.
 */
template<>
struct gl_enum_from_type<std::int8_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_BYTE;
};

template<>
struct gl_enum_from_type<std::uint8_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_UNSIGNED_BYTE;
};

template<>
struct gl_enum_from_type<std::int16_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_SHORT;
};

template<>
struct gl_enum_from_type<std::uint16_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_UNSIGNED_SHORT;
};

template<>
struct gl_enum_from_type<std::int32_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_INT;
};

template<>
struct gl_enum_from_type<std::uint32_t>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_UNSIGNED_INT;
};

template<>
struct gl_enum_from_type<float>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_FLOAT;
};

template<>
struct gl_enum_from_type<double>
{
    typedef GLenum          value_type;
    static constexpr GLenum value = GL_DOUBLE;
};


}  /* namespace mgl */

#endif /* GLENUM_HPP_ */
