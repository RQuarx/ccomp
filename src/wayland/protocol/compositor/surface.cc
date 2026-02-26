#include <wayland-server.h>

#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::surface;

#define DOMAIN "ccomp::wl::protocol::compositor"


namespace ccomp::wl::protocol
{
    static void
    attach(wl_client   *client,
           wl_resource *res,
           wl_resource *buffer,
           std::int32_t x,
           std::int32_t y) noexcept
    {
        util::get_user_data<surface>(res)->attach_buffer(client, buffer,
                                                         { x, y });
    }


    static void
    damage(wl_client   *client,
           wl_resource *res,
           std::int32_t x,
           std::int32_t y,
           std::int32_t w,
           std::int32_t h) noexcept
    {
        util::get_user_data<surface>(res)->add_redraw_rect(
            client, { x, y, w, h }, false);
    }


    static void
    frame(wl_client *client, wl_resource *res, std::uint32_t id) noexcept
    {
        util::get_user_data<surface>(res)->add_frame_callback(client, id);
    }


    static constexpr surface::impl_type impl {
        .destroy = util::destroy_client_resource,
        .attach  = attach,
        .damage  = damage,
        .frame   = frame,
    };
}


surface::state::state(wl_buffer *buff) noexcept : buffer { buff } {}


surface::surface(wl_resource *resource) noexcept
    : m_start_time { std::chrono::steady_clock::now() },
      m_resource { resource }, m_current { nullptr }
{
}


surface::~surface() {}


auto
surface::get_impl() noexcept -> const impl_type *
{
    return &impl;
}


void
surface::attach_buffer(wl_client   *client,
                       wl_resource *buffer,
                       math::point  pos) noexcept
{
    if (pos != 0)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::fatal, DOMAIN](
            "{} tried to attach a buffer with a non-zero x and y", client_tag);

        wl_resource_post_error(
            m_resource, WL_SURFACE_ERROR_INVALID_OFFSET,
            "%s tried to attach a buffer with a non-zero x and y",
            client_tag.c_str());
        return;
    }

    mf_get_pending().buffer = util::get_user_data<wl_buffer>(buffer);
}


void
surface::add_redraw_rect(wl_client          *client,
                         Cairo::RectangleInt rect,
                         bool                relative_to_buffer) noexcept
{
    if (!relative_to_buffer)
        logger[log_level::warn, DOMAIN](
            "{} is using wl_surface::damage instead of the "
            "recommended wl_surface::damage_buffer",
            util::get_client_tag(client));

    mf_get_pending().redraw_region->do_union(rect);
}


void
surface::add_frame_callback(wl_client *client, std::uint32_t id) noexcept
{
    wl_resource *cb { wl_resource_create(client, &::wl_callback_interface,
                                         wl_resource_get_version(m_resource),
                                         id) };
    if (cb == nullptr)
    {
        logger[log_level::fatal, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    m_frame_callbacks.emplace_back(cb);
}


auto
surface::mf_get_pending() noexcept -> state &
{
    if (!m_pending.has_value()) m_pending = m_current;
    return *m_pending;
}


auto
surface::mf_elapsed_ms() const noexcept -> std::int32_t
{
    auto now { std::chrono::steady_clock::now() };
    return std::chrono::duration_cast<std::chrono::milliseconds>(now
                                                                 - m_start_time)
        .count();
}
