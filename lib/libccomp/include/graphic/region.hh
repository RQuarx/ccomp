#pragma once
#include "graphic/geometry.hh"


namespace ccomp::gfx
{
    class region
    {
    public:
        void add(gfx::rect rect) noexcept;
        void subtract(gfx::rect rect) noexcept;

        void clear() noexcept;
        [[nodiscard]] auto is(gfx::rect rect) noexcept -> bool;

    private:
        std::vector<gfx::rect> m_rects;


        void mf_merge_all() noexcept;
    };
}