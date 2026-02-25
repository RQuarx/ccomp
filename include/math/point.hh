#pragma once
#include <cstdint>
#include <string>
#include <utility>


namespace ccomp::math
{
    struct point
    {
        std::int32_t x;
        std::int32_t y;


        template <typename T1, typename T2>
        constexpr point(const std::pair<T1, T2> &p)
            requires std::convertible_to<T1, std::int32_t>
                      && std::convertible_to<T2, std::int32_t>
            : x { p.first }, y { p.second }
        {
        }


        template <typename T1, typename T2>
        constexpr point(T1 x, T2 y)
            requires std::convertible_to<T1, std::int32_t>
                      && std::convertible_to<T2, std::int32_t>
            : x { x }, y { y }
        {
        }


        [[nodiscard]]
        static auto to_string() noexcept -> std::string;


        constexpr auto
        operator+=(point other) noexcept -> point &
        {
            x += other.x;
            y += other.y;
            return *this;
        }


        constexpr auto
        operator-=(point other) noexcept -> point &
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
    };


    static constexpr auto
    operator+(point a, point b) noexcept -> point
    {
        return { a.x + b.x, a.y + b.y };
    }


    static constexpr auto
    operator-(point a, point b) noexcept -> point
    {
        return { a.x - b.x, a.y - b.y };
    }


    static constexpr auto
    operator*(point a, point b) noexcept -> point
    {
        return { a.x * b.x, a.y * b.y };
    }


    static constexpr auto
    operator/(point a, point b) noexcept -> point
    {
        return { a.x / b.x, a.y / b.y };
    }


    static constexpr auto
    operator+(point a, std::int32_t b) noexcept -> point
    {
        return { a.x + b, a.y + b };
    }


    static constexpr auto
    operator-(point a, std::int32_t b) noexcept -> point
    {
        return { a.x - b, a.y - b };
    }


    static constexpr auto
    operator*(point a, std::int32_t b) noexcept -> point
    {
        return { a.x * b, a.y * b };
    }


    static constexpr auto
    operator/(point a, std::int32_t b) noexcept -> point
    {
        return { a.x / b, a.y / b };
    }
}


#ifdef CCOMP_IMPL_FORMATTER
#include <format>


template <typename CharT> struct std::formatter<ccomp::math::point, CharT>
{
    std::formatter<std::pair<std::int32_t, std::int32_t>, CharT> pair_fmt;

    constexpr auto
    parse(std::basic_format_parse_context<CharT> &ctx)
    {
        return pair_fmt.parse(ctx);
    }

    template <typename FormatContext>
    auto
    format(const ccomp::math::point &p, FormatContext &ctx)
    {
        std::pair pair { p.x, p.y };
        return pair_fmt.format(pair, ctx);
    }
};
#endif
