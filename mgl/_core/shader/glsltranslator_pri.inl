/*
 * glsltranslator_pri.inl
 *
 *  Created on: 8 avr. 2014
 *      Author: nemikolh
 */

namespace mgl {

template<>
struct glsl_translator<int>
{
    static const char* type_str()
    {
        return "int";
    }
};

template<>
struct glsl_translator<unsigned int>
{
    static const char* type_str()
    {
        return "uint";
    }
};

template<>
struct glsl_translator<bool>
{
    static const char* type_str()
    {
        return "bool";
    }
};

template<>
struct glsl_translator<float>
{
    static const char* type_str()
    {
        return "float";
    }
};

}  /* namespace mgl */
