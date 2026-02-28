#include <cstdlib>
#include <cstring>

#include "libc.hh"

namespace libc = ccomp::libc;


auto
libc::getenv(std::string_view env, std::string_view default_value) noexcept
    -> std::string_view
{
    const char *val { std::getenv(env.data()) }; /* NOLINT */
    return val == nullptr ? default_value : val;
}


auto
libc::setenv(std::string_view key,
             std::string_view value,
             bool             overwrite) noexcept -> std::optional<std::string>
{
    int retval { ::setenv(key.data(), value.data(), /* NOLINT */
                          static_cast<int>(overwrite)) };

    if (retval == 0) return std::nullopt;

    return std::strerror(errno);
}
