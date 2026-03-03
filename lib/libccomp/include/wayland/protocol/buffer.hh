#pragma once
#include <expected>

#include <wayland-server-protocol.h>

#include "wayland/core/global.hh"


namespace ccomp::wl::protocol
{
    class buffer;


    struct data_ptr_access
    {
        friend buffer;

    public:
        data_ptr_access(buffer       *buffer,
                        void         *data,
                        std::uint32_t format,
                        std::size_t   stride);

        data_ptr_access(const data_ptr_access &)                     = delete;
        auto operator=(const data_ptr_access &) -> data_ptr_access & = delete;

        data_ptr_access(data_ptr_access &&dpa) noexcept = delete;
        auto operator=(data_ptr_access &&dpa) noexcept
            -> data_ptr_access & = delete;

        ~data_ptr_access();


        [[nodiscard]] auto get_data() const noexcept -> void *;
        [[nodiscard]] auto get_format() const noexcept -> std::uint32_t;
        [[nodiscard]] auto get_stride() const noexcept -> std::size_t;

    private:
        buffer *m_buffer;

        void         *m_data;
        std::uint32_t m_format;
        std::size_t   m_stride;
    };


    struct buffer_interface
    {
        void (*destroy)(buffer *buffer) noexcept;
        auto (*get_shm)(buffer *buffer) noexcept
            -> std::expected<struct shm_buffer_data, std::string>;

        auto (*begin_data_ptr_access)(buffer *buffer, uint32_t flags) noexcept
            -> std::expected<data_ptr_access, std::string>;

        void (*end_data_ptr_access)(buffer *buffer) noexcept;
    };


    class buffer final : public core::impl_base
    {
    public:
        using impl_type = struct wl_buffer_interface;
        static constexpr core::interface_type interface {
            &::wl_buffer_interface
        };


        [[nodiscard]] static auto get_impl() noexcept -> const impl_type *;


        buffer(wl_resource *resource, buffer_interface &&interface);

        buffer(const buffer &)                     = delete;
        auto operator=(const buffer &) -> buffer & = delete;


        [[nodiscard]] auto get_resource() const noexcept -> wl_resource *;
        [[nodiscard]]
        auto get_shm() noexcept
            -> std::expected<struct shm_buffer_data, std::string>;

        [[nodiscard]]
        auto begin_data_access() noexcept
            -> std::expected<data_ptr_access, std::string>;


        void end_data_access(data_ptr_access &&data_access) noexcept;

    private:
        wl_resource     *m_resource;
        buffer_interface m_interface;
    };

    static_assert(core::impl_type<buffer>);
}
