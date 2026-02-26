#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::region;

#define DOMAIN "ccomp::wl::protocol::compositor"


namespace ccomp::wl::protocol
{
    static void
    add(wl_client   *client,
        wl_resource *res,
        std::int32_t x,
        std::int32_t y,
        std::int32_t w,
        std::int32_t h) noexcept
    {
        util::get_user_data<region>(res)->add(client, { x, y, w, h });
    }


    static void
    subtract(wl_client   *client,
             wl_resource *res,
             std::int32_t x,
             std::int32_t y,
             std::int32_t w,
             std::int32_t h) noexcept
    {
        util::get_user_data<region>(res)->remove(client, { x, y, w, h });
    }


    static constexpr region::impl_type impl {
        .destroy  = util::destroy_client_resource,
        .add      = add,
        .subtract = subtract,
    };
}


region::region(wl_resource *resource) noexcept
    : m_resource { resource }, m_region { Cairo::Region::create() }
{
}


void
region::add(wl_client * /* client */, Cairo::RectangleInt rect) noexcept
{
    m_region->do_union(rect);
}


void
region::remove(wl_client * /* client */, Cairo::RectangleInt rect) noexcept
{
    m_region->subtract(rect);
}


auto
region::get_impl() noexcept -> const region::impl_type *
{
    return &impl;
}
