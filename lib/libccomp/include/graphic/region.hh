#pragma once
#include "graphic/geometry.hh"


namespace ccomp::gfx
{
    class region
    {
    public:
        void add(gfx::rect rect) noexcept;
        void subtract(gfx::rect rect) noexcept;

    private:
        std::vector<gfx::rect> m_rects;


        void merge_all() noexcept;
    };
}