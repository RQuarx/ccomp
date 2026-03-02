#include <cstring>

#include <sys/mman.h>

#include "wayland/protocol/shm.hh"

using ccomp::wl::protocol::shm_pool;


shm_pool::memory_mapping::memory_mapping(int fd, std::size_t size)
    : size { size },
      data { mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0) }
{
    if (data == MAP_FAILED) throw std::runtime_error { std::strerror(errno) };
}


shm_pool::memory_mapping::~memory_mapping() { munmap(data, size); }


shm_pool::shm_pool(wl_resource *resource, int fd, std::int32_t size)
    : m_resource { resource },
      m_mapping { std::make_shared<memory_mapping>(fd, size) }, m_fd { fd }
{
}


void
shm_pool::on_destroy(wl_resource *res) noexcept
{
    
}