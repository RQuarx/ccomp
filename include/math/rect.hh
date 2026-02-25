#pragma once
#include <algorithm>
#include <cstdint>


namespace ccomp::math
{
    struct rect
    {
        std::int32_t x;
        std::int32_t y;
        std::int32_t w;
        std::int32_t h;


        constexpr void
        merge(rect other) noexcept
        {
            if (w == 0 || h == 0) *this = other;
            if (other.w == 0 || other.h == 0) return;

            std::int32_t x1 { std::min(x, other.x) };
            std::int32_t y1 { std::min(y, other.y) };
            std::int32_t x2 { std::max(x + w, other.x + other.w) };
            std::int32_t y2 { std::max(y + h, other.y + other.h) };

            x = x1;
            y = y1;
            w = x2 - x1;
            h = y2 - y1;
        }
    };
}
