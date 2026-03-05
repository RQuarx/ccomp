#pragma once
#include <vector>

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


        shm(std::vector<std::uint32_t> &&formats)
            : m_formats { std::move(formats) }
        {
        }


        void create_pool(wl_client    *client,
                         std::uint32_t id,
                         int           fd,
                         std::int32_t  size) noexcept;


        [[nodiscard]]
        auto get_supported_formats() const noexcept
            -> const std::vector<std::uint32_t> &;

    private:
        wl_resource               *m_resource;
        std::vector<std::uint32_t> m_formats;
    };


    struct shm_buffer_data
    {
        int           fd;
        std::int32_t  offset;
        std::int32_t  width;
        std::int32_t  height;
        std::int32_t  stride;
        wl_shm_format format;
    };


    class shm_buffer final : public core::impl_base
    {
    public:
        using impl_type = struct wl_buffer_interface;
        static constexpr core::interface_type interface {
            &::wl_buffer_interface
        };


        [[nodiscard]]
        static auto get_impl() noexcept -> const impl_type *;
        static void on_destroy(wl_resource *res);

        shm_buffer(wl_resource    *resource,
                   class shm_pool *pool,
                   shm_buffer_data data) noexcept;

    private:
        class buffer *m_base;
        wl_resource  *m_resource;

        class shm_pool *m_pool;

        shm_buffer_data m_data;
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

        shm_pool(wl_resource *resource, shm *shm, int fd, std::int32_t size);


        void resize(wl_client *client, std::int32_t size) noexcept;
        void create_buffer(wl_client      *client,
                           std::uint32_t   id,
                           shm_buffer_data data) noexcept;

    private:
        wl_resource *m_resource;
        shm         *m_shm;

        std::shared_ptr<memory_mapping> m_mapping;
        int                             m_fd;

        std::vector<std::unique_ptr<shm_buffer>> m_buffers;
    };

    static_assert(core::impl_type<shm_buffer>);
    static_assert(core::impl_type<shm_pool>);
    static_assert(core::global_type<shm>);
}
