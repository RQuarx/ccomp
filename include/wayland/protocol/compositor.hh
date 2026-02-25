#pragma once
#include <vector>

#include <wayland-server-protocol.h>

#include "math/point.hh"
#include "math/rect.hh"
#include "wayland/core/global.hh"


namespace ccomp::wl::protocol
{
    class compositor;


    namespace _impl
    {
        class surface
        {
        public:
            static constexpr int                  version { 5 };
            static constexpr core::interface_type interface {
                &::wl_surface_interface
            };


            static void on_destroy(wl_resource *resource) noexcept;


            static void destroy(wl_client   *client,
                                wl_resource *resource) noexcept;


            static void attach(wl_client   *client,
                               wl_resource *resource,
                               wl_resource *buffer,
                               std::int32_t x,
                               std::int32_t y) noexcept;


            static void damage(wl_client   *client,
                               wl_resource *resource,
                               std::int32_t x,
                               std::int32_t y,
                               std::int32_t width,
                               std::int32_t height) noexcept;


            static void frame(wl_client    *client,
                              wl_resource  *resource,
                              std::uint32_t callback) noexcept;


            static void set_opaque_region(wl_client   *client,
                                          wl_resource *resource,
                                          wl_resource *region) noexcept;


            static void set_input_region(wl_client   *client,
                                         wl_resource *resource,
                                         wl_resource *region) noexcept;


            static void commit(wl_client   *client,
                               wl_resource *resource) noexcept;


            static void set_buffer_transform(wl_client   *client,
                                             wl_resource *resource,
                                             std::int32_t transform) noexcept;


            static void set_buffer_scale(wl_client   *client,
                                         wl_resource *resource,
                                         std::int32_t scale) noexcept;


            static void damage_buffer(wl_client   *client,
                                      wl_resource *resource,
                                      std::int32_t x,
                                      std::int32_t y,
                                      std::int32_t width,
                                      std::int32_t height) noexcept;


            static void offset(wl_client   *client,
                               wl_resource *resource,
                               std::int32_t x,
                               std::int32_t y) noexcept;


            static constexpr const struct wl_surface_interface impl {
                .destroy              = surface::destroy,
                .attach               = surface::attach,
                .damage               = surface::damage,
                .frame                = surface::frame,
                .set_opaque_region    = surface::set_opaque_region,
                .set_input_region     = surface::set_input_region,
                .commit               = surface::commit,
                .set_buffer_transform = surface::set_buffer_transform,
                .set_buffer_scale     = surface::set_buffer_scale,
                .damage_buffer        = surface::damage_buffer,
                .offset               = surface::offset,
            };


            surface(compositor *compositor, wl_resource *resource) noexcept;

        private:
            wl_resource *m_buffer;
            math::rect   m_buffer_size;

            std::vector<wl_resource *> m_frame_callbacks;

            struct
            {
                wl_resource *buffer;
                math::point  offset;
                math::rect   damage;
            } m_pending;
        };


        struct region
        {
            using data_type = std::vector<math::rect>;

            static constexpr int                  version { 5 };
            static constexpr core::interface_type interface {
                &::wl_region_interface
            };


            static void on_destroy(wl_resource *resource) noexcept;


            /**
	         * destroy region
	         *
	         * Destroy the region. This will invalidate the object ID.
	         */
            static void destroy(wl_client   *client,
                                wl_resource *resource) noexcept;


            /**
	         * add rectangle to region
	         *
	         * Add the specified rectangle to the region.
	         * @param x region-local x coordinate
	         * @param y region-local y coordinate
	         * @param width rectangle width
	         * @param height rectangle height
	         */
            static void add(wl_client   *client,
                            wl_resource *resource,
                            std::int32_t x,
                            std::int32_t y,
                            std::int32_t width,
                            std::int32_t height) noexcept;


            /**
	         * subtract rectangle from region
	         *
	         * Subtract the specified rectangle from the region.
	         * @param x region-local x coordinate
	         * @param y region-local y coordinate
	         * @param width rectangle width
	         * @param height rectangle height
	         */
            static void subtract(wl_client   *client,
                                 wl_resource *resource,
                                 std::int32_t x,
                                 std::int32_t y,
                                 std::int32_t width,
                                 std::int32_t height) noexcept;

            static constexpr const struct wl_region_interface impl {
                .destroy  = region::destroy,
                .add      = region::add,
                .subtract = region::subtract,
            };
        };
    }


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

        compositor()           = default;
        ~compositor() override = default;

    private:
        /**
	     * create new surface
	     *
	     * Ask the compositor to create a new surface.
	     * @param id the new surface
	     */
        static void create_surface(wl_client    *client,
                                   wl_resource  *resource,
                                   std::uint32_t id) noexcept;


        /**
	     * create new region
	     *
	     * Ask the compositor to create a new region.
	     * @param id the new region
	     */
        static void create_region(wl_client    *client,
                                  wl_resource  *resource,
                                  std::uint32_t id) noexcept;


        static constexpr struct wl_compositor_interface impl {
            .create_surface = compositor::create_surface,
            .create_region  = compositor::create_region,
        };
    };
}
