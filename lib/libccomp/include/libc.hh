#pragma once
#include <optional>
#include <string>


namespace ccomp::libc
{
    [[nodiscard]]
    auto getenv(std::string_view env,
                std::string_view default_value = {}) noexcept
        -> std::string_view;


    auto setenv(std::string_view key,
                std::string_view value,
                bool overwrite) noexcept -> std::optional<std::string>;
}
