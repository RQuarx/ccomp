#include <wayland-server-core.h>

#include "logger.hh"
#include "wayland/core/display.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/protocol/shm.hh"


namespace ccomp
{
    [[nodiscard]]
    static auto
    get_log_threshold() noexcept -> ccomp::log_level
    {
        const char *val { std::getenv("CCOMP_LOG_THRESHOLD") };

        if (val == nullptr) return ccomp::log_level::warn;

        std::string_view level { val };
        if (level == "trace") return ccomp::log_level::trace;
        if (level == "debug") return ccomp::log_level::debug;
        if (level == "info") return ccomp::log_level::info;
        if (level == "warn") return ccomp::log_level::warn;
        if (level == "error") return ccomp::log_level::error;
        if (level == "fatal") return ccomp::log_level::fatal;
        return ccomp::log_level::warn;
    }


    class logger logger { get_log_threshold() };
}


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

    display->create_global<ccomp::wl::protocol::compositor>();
    display->create_global<ccomp::wl::protocol::shm>(
        std::vector<std::uint32_t> { WL_SHM_FORMAT_XRGB8888 });

    display->run();
}
