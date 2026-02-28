#include "graphic/region.hh"

using ccomp::gfx::region;


void
region::add(gfx::rect rect) noexcept
{
    if (rect.empty()) return;

    std::vector<gfx::rect> new_rects;
    new_rects.push_back(rect);

    for (auto &existing : m_rects)
    {
        std::vector<gfx::rect> temp;

        for (auto &nr : new_rects)
            if (existing.intersects(nr) || existing.can_merge_horizontally(nr)
                || existing.can_merge_vertically(nr))
                nr = nr.merge(existing);
            else
                temp.push_back(nr);

        new_rects = std::move(temp);
    }

    m_rects.insert(m_rects.end(), new_rects.begin(), new_rects.end());
    mf_merge_all();
}


void
region::subtract(gfx::rect rect) noexcept
{
    if (rect.empty()) return;

    std::vector<gfx::rect> new_rects;
    for (auto &existing : m_rects)
    {
        auto parts { existing.subtract(rect) };

        for (auto &p : parts)
            if (!p.empty()) new_rects.push_back(p);
    }

    m_rects = std::move(new_rects);
    mf_merge_all();
}


void
region::clear() noexcept
{
    m_rects.clear();
}


auto
region::is(gfx::rect rect) noexcept -> bool
{
    mf_merge_all();

    return m_rects.size() == 1 && m_rects.front() == rect;
}


void
region::mf_merge_all() noexcept
{
    for (std::size_t i { 0 }; i < m_rects.size(); i++)
        for (std::size_t j { i + 1 }; j < m_rects.size();)
            if (m_rects[i].intersects(m_rects[j])
                || m_rects[i].can_merge_horizontally(m_rects[j])
                || m_rects[i].can_merge_vertically(m_rects[j]))
            {
                m_rects[i] = m_rects[i].merge(m_rects[j]);
                m_rects.erase(m_rects.begin() + j);
            }
            else
                j++;
}
