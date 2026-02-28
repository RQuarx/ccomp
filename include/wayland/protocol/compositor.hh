#pragma once
#include <chrono>
#include <vector>

#include <wayland-server-protocol.h>

#include "graphic/geometry.hh"
#include "graphic/region.hh"
#include "wayland/core/global.hh"


namespace ccomp::wl::protocol
{
    class region final : public core::impl_base
    {
    public:
        using impl_type = struct wl_region_interface;
        static constexpr core::interface_type interface {
            &::wl_region_interface
        };


        [[nodiscard]]
        static auto get_impl() noexcept -> const impl_type *;


        region(wl_resource *resource) noexcept;
        ~region() override = default;


        void add(wl_client *client, gfx::rect rect) noexcept;


        void subtract(wl_client *client, gfx::rect rect) noexcept;

    private:
        wl_resource *m_resource;
        gfx::region  m_region;
    };


    class surface final : public core::impl_base
    {
        struct state
        {
            wl_buffer   *buffer { nullptr };
            gfx::point   offset;
            gfx::region  damage_region;
            gfx::size    buffer_size;
            std::int32_t buffer_scale { 1 };
            std::int32_t buffer_transform { WL_OUTPUT_TRANSFORM_NORMAL };
        };

    public:
        using impl_type = struct wl_surface_interface;
        static constexpr core::interface_type interface {
            &::wl_surface_interface
        };


        [[nodiscard]]
        static auto get_impl() noexcept -> const impl_type *;


        surface(wl_resource *resource) noexcept;
        ~surface() override;


        void attach_buffer(wl_client   *client,
                           wl_resource *buffer,
                           gfx::point   pos) noexcept;


        void add_redraw_rect(wl_client *client,
                             gfx::rect  rect,
                             bool       relative_to_buffer) noexcept;


        void add_frame_callback(wl_client *client, std::uint32_t id) noexcept;

    private:
        std::chrono::steady_clock::time_point m_start_time;
        wl_resource                          *m_resource;

        state                m_current;
        std::optional<state> m_pending;

        std::vector<wl_resource *> m_frame_callbacks;


        [[nodiscard]] auto mf_get_pending() noexcept -> state &;
        [[nodiscard]] auto mf_elapsed_ms() const noexcept -> std::int32_t;
    };


    class compositor final : public core::global_base
    {
    public:
        static constexpr int                  version { 5 };
        static constexpr core::interface_type interface {
            &::wl_compositor_interface
        };

        static void bind(wl_client    *client,
                         void         *data,
                         std::uint32_t version,
                         std::uint32_t id) noexcept;

        compositor() = default;


        [[nodiscard]]
        auto get_surfaces() noexcept -> std::vector<std::unique_ptr<surface>> &;


        [[nodiscard]]
        auto get_regions() noexcept -> std::vector<std::unique_ptr<region>> &;

    private:
        std::vector<std::unique_ptr<surface>> m_surfaces;
        std::vector<std::unique_ptr<region>>  m_regions;
    };
}
