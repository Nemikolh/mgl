/*
 * glbindbuffer.hpp
 *
 *  Created on: 16 févr. 2014
 *      Author: nemikolh
 */

#ifndef GLBINDBUFFER_HPP_
#define GLBINDBUFFER_HPP_

#include <utility>
#include <type_traits>
#include <tuple>
#include <numeric>
#include <algorithm>

#include "glbindattrib.hpp"
#include "glinstanced.hpp"

namespace mgl {

namespace priv {

template<typename T>
struct is_gl_attributes;

struct bind_buffers_helper
{
    typedef int void_;

    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    bind_buffers_helper(gl_types::id p_program_id)
        : m_program_id{p_program_id}
        , m_elements_type{0}
        , m_size{0}
        , m_size_instanced{std::numeric_limits<std::size_t>::max()}
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    // Called only if the type T is a gl attribute
    template<typename T, typename B>
    typename std::enable_if<is_gl_attributes<T>::value, void_>::type
    bind_buffer(const gl_vector<T, B>& p_buffer)
    {
        p_buffer.bind();
        gl_attribute_binder binder(m_program_id);
        gl_bind_attributes<T>::map(binder);
#ifndef MGL_NDEBUG
        assert(m_size == 0 || m_size == p_buffer.size());
#endif
        m_size = p_buffer.size();
    }

    // Called only if the type I is integral and not a gl attribute.
    template<typename I, typename B>
    typename std::enable_if<std::is_integral<I>::value && !is_gl_attributes<I>::value, void_>::type
    bind_buffer(const gl_vector<I, B>& p_buffer)
    {
        // Bind the element buffer.
        p_buffer.bind();
        m_elements_type = gl_enum_from_type<I>::value;
        // TODO : assert to check that the type of the buffer is ELEMENT_ARRAY
        // TODO : check that this function is called only once in NDEBUG mode.
    }

    // Called for simple integers, floating point or glm vectors types buffers.
    template<typename T, typename B>
    void_ bind_buffer(const gl_simple_buffer<T, B>& p_wrapper)
    {
        p_wrapper.bind();
        gl_attribute_binder binder(m_program_id);
        binder(p_wrapper.attribute_name(), tuple_size<T>::value, 0, sizeof(T), tuple_component_type<T>::value);
#ifndef MGL_NDEBUG
        assert(m_size == 0 || m_size == p_wrapper.buffer().size());
#endif
        m_size = p_wrapper.buffer().size();
    }

    // Called when the buffer is an instanced buffer.
    template<typename T, typename B>
    typename std::enable_if<is_gl_attributes<T>::value, void_>::type
    bind_buffer(const gl_instanced<gl_vector<T, B>>& p_wrapper)
    {
        p_wrapper.bind();
        gl_attribute_binder binder(m_program_id, p_wrapper.get_divisor());
        gl_bind_attributes<T>::map(binder);
        m_size_instanced = std::min(m_size_instanced, p_wrapper.size());
    }

    // Called for simple buffers.
    template<typename T, typename B>
    void_ bind_buffer(const gl_instanced<gl_simple_buffer<T, B>>& p_wrapper)
    {
        p_wrapper.bind();
        gl_attribute_binder binder(m_program_id);
        binder(p_wrapper.buffer().attribute_name(), tuple_size<T>::value, 0, sizeof(T), tuple_component_type<T>::value);
        m_size_instanced = std::min(m_size_instanced, p_wrapper.size());
    }

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    gl_types::id m_program_id;
    gl_types::en m_elements_type;
    std::size_t  m_size;
    std::size_t  m_size_instanced;
};

}  /* namespace priv */

/**
 * @class gl_bind_buffers is a class binding buffers automatically.
 */
struct gl_bind_buffers
{
    // ================================================================ //
    // =========================== CTOR/DTOR ========================== //
    // ================================================================ //

    gl_bind_buffers(gl_types::id p_program_id)
        : m_program_id(p_program_id)
    {}

    // ================================================================ //
    // ============================ METHODS =========================== //
    // ================================================================ //

    template<typename... T>
    inline
    std::tuple<gl_types::id, std::size_t, std::size_t> map(T&&... p_buffers)
    {
        //pass(bindBuffer(p_program_id, std::forward<Arg>(p_args))...);
        priv::bind_buffers_helper helper(m_program_id);
        pass(helper.bind_buffer(std::forward<T>(p_buffers))...);
        return std::make_tuple(helper.m_elements_type, helper.m_size, helper.m_size_instanced);
    }

private:

    template<typename... Args>
    void pass(Args&&...)
    {}

    // ================================================================ //
    // ============================= FIELDS =========================== //
    // ================================================================ //

    gl_types::id m_program_id;
};

}  /* namespace mgl */


#endif /* GLBINDBUFFER_HPP_ */
