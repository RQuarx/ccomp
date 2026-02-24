#pragma once
#include <wayland-server-core.h>


namespace ccomp::wl::core
{
    class event_loop_ref
    {
    public:
        event_loop_ref(wl_event_loop *loop);
        ~event_loop_ref() = default;

        event_loop_ref(const event_loop_ref &)                     = delete;
        auto operator=(const event_loop_ref &) -> event_loop_ref & = delete;

        event_loop_ref(event_loop_ref &&other) noexcept;
        auto operator=(event_loop_ref &&other) noexcept -> event_loop_ref &;

    private:
        wl_event_loop *m_loop;
    };
}
