#include <wayland-server.h>

#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::surface;

#define DOMAIN "ccomp::wl::protocol::compositor::surface"


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
        util::get_user_data<surface>(res)->add_surface_damage(client,
                                                              { x, y, w, h });
    }


    static void
    frame(wl_client *client, wl_resource *res, std::uint32_t id) noexcept
    {
        util::get_user_data<surface>(res)->add_frame_callback(client, id);
    }


    static void
    set_opaque_region(wl_client   *client,
                      wl_resource *res,
                      wl_resource *region) noexcept
    {
        util::get_user_data<surface>(res)->set_opaque_region(
            client, util::get_user_data<class region>(region));
    }


    static void
    set_input_region(wl_client   *client,
                     wl_resource *res,
                     wl_resource *region) noexcept
    {
        util::get_user_data<surface>(res)->set_input_region(
            client, util::get_user_data<class region>(region));
    }


    static void
    commit(wl_client *client, wl_resource *res) noexcept
    {
        util::get_user_data<surface>(res)->commit(client);
    }


    static void
    set_buffer_transform(wl_client   *client,
                         wl_resource *res,
                         int          transform) noexcept
    {
        util::get_user_data<surface>(res)->set_buffer_transform(client,
                                                                transform);
    }


    static void
    set_buffer_scale(wl_client   *client,
                     wl_resource *res,
                     std::int32_t scale) noexcept
    {
        util::get_user_data<surface>(res)->set_buffer_scale(client, scale);
    }


    static void
    damage_buffer(wl_client   *client,
                  wl_resource *res,
                  std::int32_t x,
                  std::int32_t y,
                  std::int32_t w,
                  std::int32_t h) noexcept
    {
        util::get_user_data<surface>(res)->add_buffer_damage(client,
                                                             { x, y, w, h });
    }


    static void
    offset(wl_client   *client,
           wl_resource *res,
           std::int32_t x,
           std::int32_t y) noexcept
    {
        util::get_user_data<surface>(res)->set_offset(client, { x, y });
    }


    static constexpr surface::impl_type impl {
        .destroy              = util::destroy_client_resource,
        .attach               = attach,
        .damage               = damage,
        .frame                = frame,
        .set_opaque_region    = set_opaque_region,
        .set_input_region     = set_input_region,
        .commit               = commit,
        .set_buffer_transform = set_buffer_transform,
        .set_buffer_scale     = set_buffer_scale,
        .damage_buffer        = damage_buffer,
        .offset               = offset,
    };
}


surface::surface(wl_resource *resource) noexcept
    : m_start_time { std::chrono::steady_clock::now() }, m_resource { resource }
{
}


auto
surface::get_impl() noexcept -> const impl_type *
{
    return &impl;
}


void
surface::attach_buffer(wl_client   *client,
                       wl_resource *buffer,
                       gfx::point   offset) noexcept
{
    if (offset != 0)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::fatal, DOMAIN](
            "{} tried to attach a buffer with a non-zero offset", client_tag);

        wl_resource_post_error(
            m_resource, WL_SURFACE_ERROR_INVALID_OFFSET,
            "%s tried to attach a buffer with a non-zero offset",
            client_tag.c_str());
        return;
    }

    mf_get_pending().buffer = util::get_user_data<wl_buffer>(buffer);
}


void
surface::add_surface_damage(wl_client *client, gfx::rect rect) noexcept
{
    logger[log_level::warn, DOMAIN](
        "{} is using wl_surface::damage instead of the "
        "recommended wl_surface::damage_buffer",
        util::get_client_tag(client));

    mf_get_pending().surface_damage.add(rect);
}


void
surface::add_buffer_damage(wl_client *client, gfx::rect rect) noexcept
{
    mf_get_pending().buffer_damage.add(rect);
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

        wl_resource_post_no_memory(m_resource);
        wl_client_post_no_memory(client);
        return;
    }

    m_frame_callbacks.emplace_back(cb);
}


void
surface::set_opaque_region(wl_client *client, region *region) noexcept
{
    if (region != nullptr)
        mf_get_pending().opaque_region = region->get_region();
    else
        mf_get_pending().opaque_region.clear();
}


void
surface::set_input_region(wl_client *client, region *region) noexcept
{
    if (region != nullptr)
        mf_get_pending().input_region = region->get_region();
    else
    {
        mf_get_pending().input_region.clear();
        mf_get_pending().input_region.add(
            { gfx::point { std::numeric_limits<std::int32_t>::min() },
              gfx::point { std::numeric_limits<std::int32_t>::max() } });
    }
}


void
surface::set_buffer_transform(wl_client *client, int transform) noexcept
{
    m_pending->buffer_transform = transform;
}


void
surface::set_buffer_scale(wl_client *client, std::int32_t scale) noexcept
{
    m_pending->buffer_scale = scale;
}


void
surface::set_offset(wl_client *client, gfx::point offset) noexcept
{
    m_pending->offset = offset;
}


void
surface::commit(wl_client *client) noexcept
{
    /* TODO */
}


auto
surface::mf_get_pending() noexcept -> state &
{
    if (!m_pending.has_value())
    {
        m_pending = {};

        m_pending->input_region.add(
            { gfx::point { std::numeric_limits<std::int32_t>::min() },
              gfx::point { std::numeric_limits<std::int32_t>::max() } });
    }

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
