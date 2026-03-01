#include "logger.hh"
#include "wayland/protocol/shm.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::shared_memory;

#define DOMAIN "ccomp::wl::protocol::shared_memory"


namespace ccomp::wl::protocol
{
    static void
    create_pool(wl_client    *client,
                wl_resource  *res,
                std::uint32_t id,
                int           fd,
                std::int32_t  size) noexcept
    {
        util::get_user_data<shared_memory>(res)->create_pool(client, id, fd,
                                                             size);
    }


    static constexpr struct wl_shm_interface impl {
        .create_pool = create_pool, .release = util::destroy_client_resource
    };
}


void
shared_memory::bind(wl_client    *client,
                    void         *data,
                    std::uint32_t version,
                    std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<shared_memory>(data) };
    auto *resource { wl_resource_create(client, interface, version, id) };

    logger[log_level::trace, DOMAIN]("binding {}", DOMAIN);

    if (resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(resource, &impl, self, nullptr);

    self->m_resource = resource;
}


void
shared_memory::create_pool(wl_client    *client,
                           std::uint32_t id,
                           int           fd,
                           std::int32_t  size) noexcept
{
    if (size <= 0)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::fatal, DOMAIN](
            "{} tried to create an shm pool with size {}", client_tag, size);

        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_STRIDE,
                               "%s tried to create an shm_pool with size %d",
                               client_tag.c_str(), size);
        close(fd);
        return;
    }

    logger[log_level::trace, DOMAIN](
        "{} with an id {} and fd {}, is creating a pool of size {}", client, id,
        fd, size);
}
