#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::_impl::surface;

#define DOMAIN "ccomp::wl::protocol::compositor"


void
surface::on_destroy(wl_resource *resource) noexcept
{
    auto *self { util::get_user_data<surface>(resource) };
    delete self;
}


void
surface::destroy(wl_client * /*client*/, wl_resource *resource) noexcept
{
    wl_resource_destroy(resource);
}


void
surface::attach(wl_client   *client,
                wl_resource *resource,
                wl_resource *buffer,
                std::int32_t x,
                std::int32_t y) noexcept
{
    if (wl_resource_get_version(resource) == 5)
        if (x != 0 || y != 0)
        {
            util::credential info;
            wl_client_get_credentials(client, GET_CREDENTIAL(info));

            logger[log_level::error, DOMAIN](
                "client[{}-{}-{}] tries to pass a non-zero x and y to "
                "surface::attach",
                info.pid, info.uid, info.gid);

            wl_resource_post_error(resource, WL_SURFACE_ERROR_INVALID_OFFSET,
                                   "passing in a non-zero x (%d) and y (%d) is "
                                   "a protocol violation",
                                   x, y);
            return;
        }

    auto *self { util::get_user_data<surface>(resource) };

    self->m_pending.buffer = buffer;
}


void
surface::damage(wl_client * /* client */,
                wl_resource *resource,
                std::int32_t x,
                std::int32_t y,
                std::int32_t w,
                std::int32_t h) noexcept
{
    auto *self { util::get_user_data<surface>(resource) };
    self->m_pending.damage.merge({ x, y, w, h }); /* NOLINT */
}


void
surface::frame(wl_client    *client,
               wl_resource  *resource,
               std::uint32_t callback) noexcept
{
    auto *self { util::get_user_data<surface>(resource) };

    wl_resource *cb { wl_resource_create(client, &::wl_callback_interface,
                                         wl_resource_get_version(resource),
                                         callback) };

    if (cb == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    self->m_frame_callbacks.emplace_back(cb);
}


void
surface::set_opaque_region(wl_client   *client,
                           wl_resource *resource,
                           wl_resource *region) noexcept
{
}
