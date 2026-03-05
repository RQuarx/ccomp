#include <utility>

#include "wayland/protocol/buffer.hh"
#include "wayland/protocol/shm.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::buffer;
using ccomp::wl::protocol::buffer_interface;
using ccomp::wl::protocol::data_ptr_access;

#define DOMAIN "ccomp::wl::protocol::buffer"


namespace ccomp::wl::protocol
{
    static constexpr struct wl_buffer_interface impl {
        .destroy = util::destroy_client_resource
    };
}


auto
buffer::get_impl() noexcept -> const impl_type *
{
    return &impl;
}


buffer::buffer(wl_resource *resource, const buffer_interface &interface)
    : m_resource { resource }, m_interface { interface }
{
}


auto
buffer::get_resource() const noexcept -> wl_resource *
{
    return m_resource;
}


auto
buffer::get_shm() noexcept -> std::expected<shm_buffer_data, std::string>
{
    return m_interface.get_shm(this);
}


auto
buffer::begin_data_access(std::uint32_t flags) noexcept
    -> std::expected<data_ptr_access, std::string>
{
    return m_interface.begin_data_ptr_access(this, flags);
}


void
buffer::end_data_access(data_ptr_access &data_access) noexcept
{
    m_interface.end_data_ptr_access(this, data_access);
    data_access.m_buffer = nullptr;
}


data_ptr_access::data_ptr_access(buffer       *buffer,
                                 void         *data,
                                 std::uint32_t format,
                                 std::size_t   stride) noexcept
    : m_buffer { buffer }, m_data { data }, m_format { format },
      m_stride { stride }
{
}


data_ptr_access::data_ptr_access(data_ptr_access &&dpa) noexcept
    : m_buffer { std::exchange(dpa.m_buffer, nullptr) },
      m_data { std::exchange(dpa.m_data, nullptr) }, m_format { dpa.m_format },
      m_stride { dpa.m_stride }
{
}


auto
data_ptr_access::operator=(data_ptr_access &&dpa) noexcept -> data_ptr_access &
{
    if (this != &dpa)
    {
        m_buffer = std::exchange(dpa.m_buffer, nullptr);
        m_data   = std::exchange(dpa.m_data, nullptr);
        m_format = dpa.m_format;
        m_stride = dpa.m_stride;
    }

    return *this;
}


data_ptr_access::~data_ptr_access()
{
    if (m_buffer != nullptr) m_buffer->end_data_access(*this);
}


auto
data_ptr_access::get_data() const noexcept -> void *
{
    return m_data;
}


auto
data_ptr_access::get_format() const noexcept -> std::uint32_t
{
    return m_format;
}


auto
data_ptr_access::get_stride() const noexcept -> std::size_t
{
    return m_stride;
}

