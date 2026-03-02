#include <filesystem>
#include <format>

#include <sys/stat.h>

#include "libc.hh"
#include "wayland/core/display.hh"

using ccomp::wl::core::display;
using namespace std::literals;

#define DOMAIN "ccomp::wayland::core::display"


namespace
{
    enum class diagnose_result : std::uint8_t
    {
        valid,
        not_set,
        not_exist,
        not_directory,
        not_owned,
        invalid_permission,
        unknown,
    };


    auto
    diagnose_path(std::string_view path) noexcept -> diagnose_result
    {
        if (path.empty()) return diagnose_result::not_set;

        const char *c_path { path.data() };
        struct stat sb;

        if (stat(c_path, &sb) != 0) return diagnose_result::not_exist;
        if (!S_ISDIR(sb.st_mode)) return diagnose_result::not_directory;
        if (sb.st_uid != getuid()) return diagnose_result::not_owned;
        if ((sb.st_mode & 0777) != 0700)
            return diagnose_result::invalid_permission;

        return diagnose_result::valid;
    }


    auto
    get_error_message(diagnose_result err, std::string_view path) noexcept
        -> std::string
    {
        switch (err)
        {
        case diagnose_result::not_set:
            return std::format(
                "failed to add a wayland display socket: {} is not set", path);
        case diagnose_result::not_exist:
            return std::format(
                "failed to add a wayland display socket: {} doesn't exist",
                path);
        case diagnose_result::not_directory:
            return std::format(
                "failed to add a wayland display socket: {} is not a directory",
                path);
        case diagnose_result::not_owned:
            return std::format("failed to add a wayland display socket: {} is "
                               "not owned by the current user",
                               path);
        case diagnose_result::invalid_permission:
            return std::format("failed to add a wayland display socket: {} has "
                               "invalid permissions",
                               path);
        default: return "failed to add a wayland display socket";
        }
    }
}


auto
display::create() noexcept -> std::optional<display>
{
    display d { wl_display_create() };

    if (d.m_display == nullptr)
    {
        logger[log_level::fatal, DOMAIN]("failed to create a wayland display");
        return std::nullopt;
    }

    return std::move(d);
}


display::display(wl_display *display) noexcept : m_display { display } {}


display::~display()
{
    if (m_display != nullptr)
    {
        wl_display_destroy_clients(m_display);
        wl_display_destroy(m_display);
    }
}


display::display(display &&other) noexcept : m_display { other.m_display }
{
    other.m_display = nullptr;
}


auto
display::operator=(display &&other) noexcept -> display &
{
    if (&other != this)
        m_display = std::exchange(other.m_display, nullptr);

    return *this;
}


auto
display::add_socket(const char *name) noexcept
    -> std::optional<std::string_view>
{
    std::string_view socket_name { name == nullptr
                                       ? libc::getenv("WAYLAND_DISPLAY")
                                       : name };

    if (wl_display_add_socket(m_display, name) == 0)
    {
        if (socket_name.empty()) socket_name = "wayland-1";
        logger[log_level::info, DOMAIN]("added wayland display socket on {}",
                                        socket_name);
        return socket_name;
    }

    bool is_absolute { std::filesystem::path { socket_name }.is_absolute() };
    std::string_view to_be_diagnosed;
    std::string_view diagnosed_variable;
    diagnose_result  result { diagnose_result::valid };

    if (socket_name.empty())
    {
        to_be_diagnosed    = libc::getenv("XDG_RUNTIME_DIR");
        diagnosed_variable = "$XDG_RUNTIME_DIR";
    }
    else if (is_absolute)
    {
        to_be_diagnosed    = socket_name;
        diagnosed_variable = socket_name;
    }
    else
        result = diagnose_result::unknown;

    if (result == diagnose_result::valid)
        result = diagnose_path(to_be_diagnosed);

    logger[log_level::fatal, DOMAIN](
        "{}", get_error_message(result, diagnosed_variable));

    return std::nullopt;
}


void
display::run() noexcept
{
    wl_display_run(m_display);
}


auto
display::get_event_loop() noexcept -> event_loop_ref
{
    return wl_display_get_event_loop(m_display);
}


auto
display::raw() noexcept -> wl_display *
{
    return m_display;
}