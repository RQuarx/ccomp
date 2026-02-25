#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::compositor;

#define DOMAIN "ccomp::wl::protocol::compositor"


void
compositor::bind(wl_client    *client,
                 void         *data,
                 std::uint32_t version,
                 std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<compositor>(data) };
    auto *resource { wl_resource_create(client, interface, version, id) };

    if (resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(resource, &impl, self, nullptr);
}


void
compositor::create_surface(wl_client    *client,
                           wl_resource  *resource,
                           std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<compositor>(resource) };

    auto *surface_resource { wl_resource_create(
        client, _impl::surface::interface, wl_resource_get_version(resource),
        id) };

    if (surface_resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    auto *data {
        new (std::nothrow) _impl::surface { self, surface_resource }
    };

    if (data == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a surface (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(surface_resource, &_impl::surface::impl,
                                   data, _impl::surface::on_destroy);
}


void
compositor::create_region(wl_client    *client,
                          wl_resource  *resource,
                          std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<compositor>(resource) };

    auto *region_resource { wl_resource_create(
        client, _impl::region::interface, wl_resource_get_version(resource),
        id) };

    if (region_resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    auto *data { new (std::nothrow) _impl::region::data_type };

    if (data == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a region (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(region_resource, &_impl::region::impl, data,
                                   _impl::region::on_destroy);
}
