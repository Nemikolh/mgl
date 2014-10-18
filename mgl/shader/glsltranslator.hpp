/*
 * gltranslator.hpp
 *
 *  Created on: 24 sept. 2013
 *      Author: nemikolh
 */

#ifndef GLTRANSLATOR_HPP_
#define GLTRANSLATOR_HPP_

namespace mgl {

/**
 * \class glsl_translator Offer translation for a type.
 */
template<typename T>
struct glsl_translator;

} /* namespace mgl. */

/**
 * Local macro to ease the definition of the translators.
 */
#define DEFINE_TRANSLATOR(name)       \
    template<>                          \
    struct glsl_translator<glm::name>     \
    {                                   \
        static const char * type_str()  \
        {                               \
                return #name;           \
        }                               \
    }

#ifdef GLM_VERSION
// Inclusion of the translation for vector types.
#include "glsltranslator_vec.inl"
#include "glsltranslator_mat.inl"
#else
#warning "MGL WARN: Translators not defined, GLM types not included."
#endif
// Translation for primitives types.
#include "glsltranslator_pri.inl"

#undef DEFINE_TRANSLATOR

#endif /* GLTRANSLATOR_HPP_ */
