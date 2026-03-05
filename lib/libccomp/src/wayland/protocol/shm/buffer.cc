#include "wayland/protocol/shm.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::shm_buffer;

#define DOMAIN "ccomp::wl::protocol::shm::shm_buffer"


namespace ccomp::wl::protocol
{
    constexpr const shm_buffer::impl_type impl {
        .destroy = util::destroy_client_resource
    };

    constexpr const buffer_interface buffer_impl {
        .destroy               = nullptr,
        .get_shm               = nullptr,
        .begin_data_ptr_access = nullptr,
        .end_data_ptr_access   = nullptr,
    };
}


auto
shm_buffer::get_impl() noexcept -> const impl_type *
{
    return &impl;
}


void
shm_buffer::on_destroy(wl_resource *res)
{
}

shm_buffer::shm_buffer(wl_resource    *resource,
                       class shm_pool *pool,
                       shm_buffer_data data) noexcept
    : m_base { resource, buffer_impl }, m_resource { resource },
      m_pool { pool }, m_data { data }
{
}
