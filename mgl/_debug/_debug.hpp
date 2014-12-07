/*
 * _debug.hpp
 *
 *  Created on: 22 juin 2013
 *      Author: nemikolh
 */

#ifndef MGL_DEBUG_HPP_
#define MGL_DEBUG_HPP_

#ifndef MGL_NDEBUG
#include <cassert>
#endif

namespace mgl {

/*
 * Dirty Macro to perform check only on debug mode.
 * This code comes from the SFML GLCheck.hpp file.
 */
#ifndef MGL_NDEBUG

    // In debug mode, perform a test on every OpenGL call
    #define glCheck(call) ((call), priv::glCheckError(__FILE__, __LINE__))

#else

    // Else, we don't add any overhead
    #define glCheck(call) (call)

#endif

// ================================================================ //
// ========================= PRIV NAMESPACE ======================= //
// ================================================================ //

namespace priv {

/**
 * \brief Check the last OpenGL error
 * \param file Source file where the call is located
 * \param line Line number of the source file where the call is located
 */
bool glCheckError(const char* file, unsigned int line);

/**
 * \brief Same as glCheck but throw an exception if an error occured.
 */
void glTryError();

} /* namespace priv */


} /* namespace mgl */

#endif /* MGL_DEBUG_HPP_ */
