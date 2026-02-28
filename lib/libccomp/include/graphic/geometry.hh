#pragma once
#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>


namespace ccomp::gfx
{
    template <typename T> using tpair = std::pair<T, T>;


    struct point
    {
        std::int32_t x;
        std::int32_t y;


        constexpr point() noexcept = default;
        constexpr point(std::int32_t all) noexcept : x { all }, y { all } {}
        constexpr point(std::int32_t x, std::int32_t y) noexcept
            : x { x }, y { y }
        {
        }
        constexpr point(tpair<std::int32_t> pos) noexcept
            : x { pos.first }, y { pos.second }
        {
        }


        [[nodiscard]]
        constexpr auto
        operator+(point other) const noexcept -> point
        {
            return { other.x + x, other.y + y };
        }


        [[nodiscard]]
        constexpr auto
        operator-(point other) const noexcept -> point
        {
            return { other.x - x, other.y - y };
        }


        [[nodiscard]]
        constexpr auto
        operator*(point other) const noexcept -> point
        {
            return { other.x * x, other.y * y };
        }


        [[nodiscard]]
        constexpr auto
        operator/(point other) const noexcept -> point
        {
            return { other.x / x, other.y / y };
        }


        [[nodiscard]]
        constexpr auto
        operator+=(point other) noexcept -> point &
        {
            x += other.x;
            y += other.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator-=(point other) noexcept -> point &
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator*=(point other) noexcept -> point &
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator/=(point other) noexcept -> point &
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator==(point other) const noexcept -> bool
        {
            return x == other.x && y == other.y;
        }


        [[nodiscard]]
        constexpr auto
        operator==(std::int32_t other) const noexcept -> bool
        {
            return x == other && y == other;
        }


        [[nodiscard]]
        constexpr auto
        operator!=(point other) const noexcept -> bool
        {
            return !(*this == other);
        }


        [[nodiscard]]
        constexpr auto
        operator!=(std::int32_t other) const noexcept -> bool
        {
            return !(*this == other);
        }
    };


    struct size
    {
        std::int32_t w;
        std::int32_t h;


        constexpr size() noexcept = default;
        constexpr size(std::int32_t all) noexcept : w { all }, h { all } {}
        constexpr size(std::int32_t w, std::int32_t h) noexcept
            : w { w }, h { h }
        {
        }
        constexpr size(tpair<std::int32_t> pos) noexcept
            : w { pos.first }, h { pos.second }
        {
        }


        [[nodiscard]]
        constexpr auto
        operator+(size other) const noexcept -> size
        {
            return { other.w + w, other.h + h };
        }


        [[nodiscard]]
        constexpr auto
        operator-(size other) const noexcept -> size
        {
            return { other.w - w, other.h - h };
        }


        [[nodiscard]]
        constexpr auto
        operator*(size other) const noexcept -> size
        {
            return { other.w * w, other.h * h };
        }


        [[nodiscard]]
        constexpr auto
        operator/(size other) const noexcept -> size
        {
            return { other.w / w, other.h / h };
        }


        [[nodiscard]]
        constexpr auto
        operator+=(size other) noexcept -> size &
        {
            w += other.w;
            h += other.h;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator-=(size other) noexcept -> size &
        {
            w -= other.w;
            h -= other.h;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator*=(size other) noexcept -> size &
        {
            w *= other.w;
            h *= other.h;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator/=(size other) noexcept -> size &
        {
            w /= other.w;
            h /= other.h;
            return *this;
        }
    };


    struct rect
    {
        std::int32_t x1;
        std::int32_t y1;
        std::int32_t x2;
        std::int32_t y2;


        constexpr rect() noexcept = default;
        constexpr rect(std::int32_t x,
                       std::int32_t y,
                       std::int32_t w,
                       std::int32_t h) noexcept
            : x1 { x }, y1 { y }, x2 { x + w }, y2 { y + h }
        {
        }

        constexpr rect(point pos, size s) noexcept
            : x1 { pos.x }, y1 { pos.y }, x2 { pos.x + s.w }, y2 { pos.y + s.h }
        {
        }

        constexpr rect(point p1, point p2) noexcept
            : x1 { p1.x }, y1 { p1.y }, x2 { p2.x }, y2 { p2.y }
        {
        }


        [[nodiscard]]
        constexpr auto
        width() const noexcept -> std::int32_t
        {
            return x2 - x1;
        }


        [[nodiscard]]
        constexpr auto
        height() const noexcept -> std::int32_t
        {
            return y2 - y1;
        }


        [[nodiscard]]
        constexpr auto
        size() const noexcept -> gfx::size
        {
            return { width(), height() };
        }


        [[nodiscard]]
        constexpr auto
        position() const noexcept -> point
        {
            return { x1, y1 };
        }


        [[nodiscard]]
        constexpr auto
        empty() const noexcept -> bool
        {
            return width() <= 0 || height() <= 0;
        }


        [[nodiscard]]
        constexpr auto
        contains(point p) const noexcept -> bool
        {
            return p.x >= x1 && p.x < x2 && p.y >= y1 && p.y < y2;
        }


        [[nodiscard]]
        constexpr auto
        contains(rect other) const noexcept -> bool
        {
            return other.x1 >= x1 && other.y1 >= y1 && other.x2 <= x2
                && other.y2 <= y2;
        }


        [[nodiscard]]
        constexpr auto
        intersects(rect other) const noexcept -> bool
        {
            return other.x2 > x1 && other.x1 < x2 && other.y2 > y1
                && other.y1 < y2;
        }


        [[nodiscard]]
        constexpr auto
        intersection(rect other) const noexcept -> rect
        {
            return { std::max(x1, other.x1), std::max(y1, other.y1),
                     std::min(x2, other.x2), std::min(y2, other.y2) };
        }


        [[nodiscard]]
        constexpr auto
        translated(point delta) const noexcept -> rect
        {
            return { x1 + delta.x, y1 + delta.y, width(), height() };
        }


        [[nodiscard]]
        constexpr auto
        can_merge_horizontally(const rect &other) const noexcept -> bool
        {
            return y1 == other.y1 && y2 == other.y2
                && (x2 == other.x1 || other.x2 == x1);
        }


        [[nodiscard]]
        constexpr auto
        can_merge_vertically(const rect &other) const noexcept -> bool
        {
            return x1 == other.x1 && x2 == other.x2
                && (y2 == other.y1 || other.y2 == y1);
        }


        [[nodiscard]]
        constexpr auto
        merge(const rect &other) const noexcept -> rect
        {
            return { std::min(x1, other.x1), std::min(y1, other.y1),
                     std::max(x2, other.x2), std::max(y2, other.y2) };
        }


        [[nodiscard]]
        constexpr auto
        subtract(rect other) const noexcept -> std::vector<gfx::rect>
        {
            std::vector<gfx::rect> out { 4 };

            if (!intersects(other))
            {
                out[0] = *this;
                return out;
            }

            rect        i { intersection(other) };
            std::size_t count { 0 };

            if (y1 < i.y1) out[count++] = { x1, y1, x2, i.y1 };
            if (i.y2 < y2) out[count++] = { x1, i.y2, x2, y2 };
            if (x1 < i.x1) out[count++] = { x1, i.y1, i.x1, i.y2 };
            if (i.x2 < x2) out[count++] = { i.x2, i.y1, x2, i.y2 };

            return out;
        }


        [[nodiscard]]
        constexpr auto
        operator+=(point delta) noexcept -> rect &
        {
            x1 += delta.x;
            y1 += delta.y;
            x2 += delta.x;
            y2 += delta.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator-=(point delta) noexcept -> rect &
        {
            x1 -= delta.x;
            y1 -= delta.y;
            x2 -= delta.x;
            y2 -= delta.y;
            return *this;
        }


        [[nodiscard]]
        constexpr auto
        operator==(gfx::rect other) const noexcept -> bool
        {
            return x1 == other.x1 && x2 == other.x2 && y1 == other.y1
                && y2 == other.y2;
        }


        [[nodiscard]]
        constexpr auto
        operator!=(gfx::rect other) const noexcept -> bool
        {
            return !(*this == other);
        }
    };
}
