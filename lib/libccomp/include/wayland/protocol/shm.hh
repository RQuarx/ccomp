#pragma once
#include <wayland-server-protocol.h>

#include "wayland/core/global.hh"


namespace ccomp::wl::protocol
{
    class shm final : public core::global_base
    {
    public:
        static constexpr int                  version { 2 };
        static constexpr core::interface_type interface { &::wl_shm_interface };

        static void bind(wl_client    *client,
                         void         *data,
                         std::uint32_t version,
                         std::uint32_t id) noexcept;


        void create_pool(wl_client    *client,
                         std::uint32_t id,
                         int           fd,
                         std::int32_t  size) noexcept;

    private:
        wl_resource *m_resource;
    };


    class shm_pool final : public core::impl_base
    {
        struct memory_mapping
        {
            std::size_t size;
            void       *data;


            memory_mapping(int fd, std::size_t size);
            ~memory_mapping();


            memory_mapping(const memory_mapping &)                     = delete;
            auto operator=(const memory_mapping &) -> memory_mapping & = delete;
        };

    public:
        using impl_type = struct wl_shm_pool_interface;
        static constexpr core::interface_type interface {
            &::wl_shm_pool_interface
        };


        [[nodiscard]]
        static auto get_impl() noexcept -> const impl_type *;
        static void on_destroy(wl_resource *res) noexcept;

        shm_pool(wl_resource *resource, int fd, std::int32_t size);

    private:
        wl_resource                    *m_resource;
        std::shared_ptr<memory_mapping> m_mapping;
        int                             m_fd;
    };
}
