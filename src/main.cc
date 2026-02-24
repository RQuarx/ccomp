#include <wayland-server-core.h>

#include "logger.hh"
#include "wayland/core/display.hh"


namespace
{
    void
    wl_log_handler(const char *fmt, va_list args)
    {
        std::array<char, 1024> buf;
        std::vsnprintf(buf.data(), buf.size(), fmt, args);

        std::string_view msg { buf.data() };
        if (msg.back() == '\n') msg.remove_suffix(1);

        ccomp::logger[ccomp::log_level::debug, "wayland"]("{}", msg);
    }
}


auto
main(int argc, char **argv) noexcept -> int
{
    wl_log_set_handler_server(wl_log_handler);

    auto display { ccomp::wl::core::display::create() };
    if (!display) return 1;

    auto sock_name { display->add_socket(nullptr) };
    if (!sock_name) return 1;

    display->run();
}
