#include "logger.hh"
#include "wayland/protocol/shm.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::shm;

#define DOMAIN "ccomp::wl::protocol::shm"


namespace ccomp::wl::protocol
{
    static void
    create_pool(wl_client    *client,
                wl_resource  *res,
                std::uint32_t id,
                int           fd,
                std::int32_t  size) noexcept
    {
        util::get_user_data<shm>(res)->create_pool(client, id, fd, size);
    }


    static constexpr struct wl_shm_interface impl {
        .create_pool = create_pool, .release = util::destroy_client_resource
    };
}


void
shm::bind(wl_client    *client,
          void         *data,
          std::uint32_t version,
          std::uint32_t id) noexcept
{
    auto *self { util::get_user_data<shm>(data) };
    auto *resource { wl_resource_create(client, interface, version, id) };

    logger[log_level::trace, DOMAIN]("binding {}", DOMAIN);

    if (resource == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");

        wl_resource_post_no_memory(resource);
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(resource, &impl, self, nullptr);

    self->m_resource = resource;
}


void
shm::create_pool(wl_client    *client,
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

    wl_resource *res { wl_resource_create(client, shm::interface, shm::version,
                                          id) };

    if (res == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    shm_pool *pool { nullptr };

    try
    {
        pool = new shm_pool { res, fd, size };
    }
    catch (const std::runtime_error &e)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN]("{} {}", client_tag, e.what());
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_FD, "%s %s",
                               client_tag.c_str(), e.what());

        return;
    }
    catch (const std::bad_alloc &e)
    {
        logger[log_level::error, DOMAIN](
            "failed to create an shm_pool object (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(res, shm_pool::get_impl(), pool,
                                   shm_pool::on_destroy);
}
