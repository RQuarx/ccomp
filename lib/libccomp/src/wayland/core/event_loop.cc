#include <utility>

#include "wayland/core/event_loop.hh"

using ccomp::wl::core::event_loop_ref;


event_loop_ref::event_loop_ref(wl_event_loop *loop) : m_loop { loop } {}


event_loop_ref::event_loop_ref(event_loop_ref &&other) noexcept
    : m_loop { other.m_loop }
{
    other.m_loop = nullptr;
}


auto
event_loop_ref::operator=(event_loop_ref &&other) noexcept -> event_loop_ref &
{
    if (&other != this) m_loop = std::exchange(other.m_loop, nullptr);
    return *this;
}



