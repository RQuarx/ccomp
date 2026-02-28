#include <utility>

#include "wayland/core/global.hh"

using ccomp::wl::core::global;


global::~global() { wl_global_destroy(m_global); }


global::global(global &&other) noexcept
    : m_instance { std::move(other.m_instance) }, m_display { other.m_display },
      m_global { std::exchange(other.m_global, nullptr) },
      m_interface { other.m_interface }, m_version { other.m_version }
{
}


auto
global::operator=(global &&other) noexcept -> global &
{
    if (this == &other) return *this;

    if (m_global != nullptr) wl_global_destroy(m_global);

    m_global    = std::exchange(other.m_global, nullptr);
    m_instance  = std::move(other.m_instance);
    m_interface = other.m_interface;
    m_version   = other.m_version;

    return *this;
}


auto
global::get_display() const noexcept -> class display &
{
    return m_display;
}


auto
global::get_version() const noexcept -> int
{
    return m_version;
}


auto
global::get_instance() const noexcept -> global_base *
{
    return m_instance.get();
}


auto
global::get_interface() const noexcept -> const struct wl_interface *
{
    return m_interface;
}
