#pragma once
#include <wayland-server-protocol.h>

#include "wayland/core/global.hh"


namespace ccomp::wl::protocol
{
    class shm_pool final : public core::impl_base
    {
    public:
        using impl_type = struct wl_shm_pool_interface;
        static constexpr core::interface_type interface {
            &::wl_shm_pool_interface
        };


        [[nodiscard]] static auto get_impl() noexcept -> const impl_type *;

        shm_pool(wl_resource *resource) noexcept;

    private:
    };


    class shared_memory final : public core::global_base
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
}
