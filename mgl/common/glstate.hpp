/*
 * glstate.hpp
 *
 *  Created on: 28 oct. 2014
 *      Author: nemikolh
 */

#ifndef MGL_TYPE_GLSTATE_HPP_
#define MGL_TYPE_GLSTATE_HPP_

#include <type_traits>

namespace mgl {

// -----------------------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------------------------------------------------//

/**
 * \class has_typedef_gl_object_type test for the presence or absence of the nested typedef
 * gl_object_type.
 * This is a conformant integral metafunction that can be used with the boost::mpl library.
 */
template<typename T>
class has_typedef_gl_object_type
{
    template<typename U>
    static std::true_type  deduce(U*, typename U::gl_object_type* = 0);
    static std::false_type deduce(...);

public:

    typedef decltype(deduce(static_cast<T*>(0))) type;
    static constexpr bool value = type::value;
};

/**
 * \class has_save_restore_state test for the presence of the static functions save_state() and restore_state().
 * This is a conformant integral metafunction that can be used with the boost::mpl library.
 */
template<typename T>
class has_save_restore_state
{
    template<typename F, F f> class Helper{};

    template<typename U>
    static std::true_type  deduce(U*, Helper<void (*)(), &U::save_state>* = 0, Helper<void (*)(), &U::restore_state>* = 0);
    static std::false_type deduce(...);

public:

    typedef decltype(deduce(static_cast<T*>(0))) type;
    static constexpr bool value = type::value;
};

/* namespace priv. */
namespace priv {

template<typename GLState, bool = has_save_restore_state<GLState>::value >
struct gl_state_traits
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    /**
     * \brief Save the current state if a gl_object_type typedef
     * is provided.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<has_typedef_gl_object_type<U>::value, void>::type
    save_state()
    {
        U::gl_object_type::save_state();
    }

    /**
     * \brief Does nothing.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<!has_typedef_gl_object_type<U>::value, void>::type
    save_state()
    {}

    /**
     * \brief Restore the current state if a gl_object_type typedef
     * is provided.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<has_typedef_gl_object_type<U>::value, void>::type
    restore_state()
    {
        U::gl_object_type::restore_state();
    }

    /**
     * \brief Does nothing.
     */
    template<typename U = GLState>
    static inline constexpr
    typename std::enable_if<!has_typedef_gl_object_type<U>::value, void>::type
    restore_state()
    {}
};

template<typename GLState>
struct gl_state_traits<GLState, true>
{
    // ================================================================ //
    // ======================== STATIC METHODS ======================== //
    // ================================================================ //

    /**
     * \brief Save the state using the static functions provided.
     */
    static inline constexpr void save_state()
    {
        GLState::save_state();
    }

    /**
     * \brief Restore the state using the static functions provided.
     */
    static inline constexpr void restore_state()
    {
        GLState::restore_state();
    }

};

} /* namespace priv. */

/**
 * \class gl_state_traits is an helper for storing gl_state.
 * This class is mainly used for the gl_scope class.
 * If you want to preserve an OpenGL state you have the following options :
 *
 *      1.  You can provide a partial template specialization of gl_state_traits (it'll save a bit of compile time template instantiation )
 *
 *      2. You can provide nested static functions that performs all the job, named
 *        \code save_state(); \endcode and \code restore_state(); \endcode
 *
 *      3. You can provide the nested typedef ::gl_object_type which can be one of the following
 *          * gl_object_buffer
 *          * gl_object_framebuffer
 *          * gl_object_program
 *          * gl_object_shader
 *          * gl_object_texture
 *          * gl_object_transformfeedback
 *          * gl_object_vertexarrays
 *
 *      4. And finally you can do nothing at all if you don't want any state preservation.
 *
 * You can also provide all those options and they'll be picked up in this order.
 */
template<typename GLState>
struct gl_state_traits : public priv::gl_state_traits<GLState>
{};

} /* namespace mgl */



#endif /* MGL_TYPE_GLSTATE_HPP_ */
