#pragma once
#include <optional>
#include <string_view>

#include <wayland-server-core.h>

#include "wayland/core/event_loop.hh"


namespace ccomp::wl::core
{
    class display
    {
    public:
        static auto create() noexcept -> std::optional<display>;
        ~display();


        display(const display &)                     = delete;
        auto operator=(const display &) -> display & = delete;

        display(display &&other) noexcept;
        auto operator=(display &&other) noexcept -> display &;


        auto add_socket(const char *name) noexcept
            -> std::optional<std::string_view>;


        void run() noexcept;


        [[nodiscard]] auto get_event_loop() noexcept -> event_loop_ref;

    private:
        wl_display *m_display;


        display(wl_display *display) noexcept;
    };
}
