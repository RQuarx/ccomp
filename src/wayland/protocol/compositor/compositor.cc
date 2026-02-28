#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::compositor;

#define DOMAIN "ccomp::wl::protocol::compositor"


namespace ccomp::wl::protocol
{
    template <core::impl_type T>
    static void
    create_instance(wl_client    *client,
                    wl_resource  *resource,
                    std::uint32_t id) noexcept
    {
        auto *self { util::get_user_data<compositor>(resource) };
        auto *res { wl_resource_create(client, T::interface,
                                       wl_resource_get_version(resource), id) };

        if (res == nullptr)
        {
            logger[log_level::error, "ccomp::wl::protocol::compositor"](
                "failed to create a wl_resource (low on memory?)");
            wl_client_post_no_memory(client);
            return;
        }


        std::erase(self->get_surfaces(), nullptr);
        std::erase(self->get_regions(), nullptr);

        T *data;

        if constexpr (std::is_same_v<T, surface>)
        {
            data = self->get_surfaces()
                       .emplace_back(std::move(std::make_unique<surface>(res)))
                       .get();
        }
        else
        {
            data = self->get_regions()
                       .emplace_back(std::move(std::make_unique<region>(res)))
                       .get();
        }

        if (data == nullptr)
        {
            logger[log_level::error, "ccomp::wl::protocol::compositor"](
                "failed to create a compositor data (low on memory?)");
            wl_client_post_no_memory(client);
            return;
        }

        wl_resource_set_implementation(res, T::get_impl(), data, nullptr);
    }


    static constexpr struct wl_compositor_interface impl {
        .create_surface = create_instance<surface>,
        .create_region  = create_instance<region>,
    };
}


void
compositor::bind(wl_client    *client,
                 void         *data,
                 std::uint32_t version,
                 std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<compositor>(data) };
    auto *resource { wl_resource_create(client, interface, version, id) };

    logger[log_level::trace, DOMAIN]("binding ccomp::wl::protocol::compositor");

    if (resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(resource, &impl, self, nullptr);
}


auto
compositor::get_surfaces() noexcept -> std::vector<std::unique_ptr<surface>> &
{
    return m_surfaces;
}


auto
compositor::get_regions() noexcept -> std::vector<std::unique_ptr<region>> &
{
    return m_regions;
}
