#include <algorithm>
#include <cstring>

#include <sys/mman.h>

#include "graphic/pixel_format.hh"
#include "logger.hh"
#include "wayland/protocol/shm.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::shm_pool;

#define DOMAIN "ccomp::wl::protocol::shm::shm_pool"


namespace ccomp::wl::protocol
{
    static void
    create_buffer(wl_client    *client,
                  wl_resource  *res,
                  std::uint32_t id,
                  std::int32_t  offset,
                  std::int32_t  width,
                  std::int32_t  height,
                  std::int32_t  stride,
                  std::uint32_t format) noexcept
    {
        util::get_user_data<shm_pool>(res)->create_buffer(
            client, id,
            { .fd     = -1,
              .offset = offset,
              .width  = width,
              .height = height,
              .stride = stride,
              .format = static_cast<wl_shm_format>(format) });
    }


    static void
    resize(struct wl_client   *client,
           struct wl_resource *res,
           int32_t             size) noexcept
    {
        util::get_user_data<shm_pool>(res)->resize(client, size);
    }


    static constexpr shm_pool::impl_type impl { .create_buffer = create_buffer,
                                                .destroy
                                                = util::destroy_client_resource,
                                                .resize = resize };
}


shm_pool::memory_mapping::memory_mapping(int fd, std::size_t size)
    : size { size },
      data { mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0) }
{
    if (data == MAP_FAILED) throw std::runtime_error { std::strerror(errno) };
}


shm_pool::memory_mapping::~memory_mapping() { munmap(data, size); }


shm_pool::shm_pool(wl_resource *resource, shm *shm, int fd, std::int32_t size)
try
    : m_resource{ resource }, m_shm{ shm },
      m_mapping{ std::make_shared<memory_mapping>(fd, size) }, m_fd{ fd }
{
}
catch (const std::runtime_error &e)
{
    logger[log_level::error, DOMAIN]("failed to create an mmap: {}", e.what());
    wl_resource_post_error(resource, WL_SHM_ERROR_INVALID_FD,
                           "failed to create an mmap: %s", e.what());
}


auto
shm_pool::get_impl() noexcept -> const impl_type *
{
    return &impl;
}


void
shm_pool::on_destroy(wl_resource *res) noexcept
{
    util::get_user_data<shm_pool>(res)->m_resource = nullptr;
}


void
shm_pool::create_buffer(wl_client      *client,
                        std::uint32_t   id,
                        shm_buffer_data data) noexcept
{
    if (data.offset < 0 || data.width <= 0 || data.height <= 0
        || data.stride < data.width
        || data.offset + (static_cast<std::size_t>(data.stride) * data.height)
               > m_mapping->size)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN](
            "{} tried to pass a buffer data with invalid values", client_tag);
        wl_resource_post_error(
            m_resource, WL_SHM_ERROR_INVALID_STRIDE,
            "%s tried to pass a buffer data with invalid values",
            client_tag.c_str());
        return;
    }

    if (!std::ranges::contains(m_shm->get_supported_formats(), data.format))
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN](
            "{} tried to use an unsupported format", client_tag);
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_STRIDE,
                               "%s tried to use an unsupported format",
                               client_tag.c_str());
        return;
    }

    std::uint32_t drm_format { gfx::convert_wl_shm_format_to_drm(data.format) };
    auto format_info { gfx::pixel_format_info::get_format_info(drm_format) };

    if (format_info.is_invalid())
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN]("{} tried to use an unknown format",
                                         client_tag);
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_STRIDE,
                               "%s tried to use an unknown format",
                               client_tag.c_str());
        return;
    }

    if (!format_info.check_stride(data.stride, data.width))
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN]("{} tried to use an invalid stride {}",
                                         client_tag, data.stride);
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_STRIDE,
                               "%s tried to use an invalid stride %d",
                               client_tag.c_str(), data.stride);
        return;
    }

    wl_resource *res { wl_resource_create(client, shm_buffer::interface, 1,
                                          id) };
    if (res == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a wl_resource object (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    auto *buff {
        new (std::nothrow) shm_buffer { res, this, data }
    };
    if (buff == nullptr)
    {
        logger[log_level::error, DOMAIN](
            "failed to create a shm_buffer object (low on memory?)");
        wl_client_post_no_memory(client);
        return;
    }

    wl_resource_set_implementation(res, shm_buffer::get_impl(), buff,
                                   shm_buffer::on_destroy);
}


void
shm_pool::resize(wl_client *client, std::int32_t size) noexcept
{
    if (size <= 0 || static_cast<std::size_t>(size) < m_mapping->size)
    {
        std::string client_tag { util::get_client_tag(client) };

        logger[log_level::error, DOMAIN](
            "{} tried to shrink a pool, which is forbidden ({} to {})",
            client_tag, m_mapping->size, size);
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_STRIDE,
                               "%s tried to tried to shrink a pool, which is "
                               "forbidden (%zu to %d)",
                               client_tag.c_str(), m_mapping->size, size);
        return;
    }


    try
    {
        m_mapping = std::make_unique<memory_mapping>(m_fd, size);
    }
    catch (const std::runtime_error &e)
    {
        logger[log_level::error, DOMAIN]("failed to create an mmap: {}",
                                         e.what());
        wl_resource_post_error(m_resource, WL_SHM_ERROR_INVALID_FD,
                               "failed to create an mmap: %s", e.what());
        return;
    }
}
