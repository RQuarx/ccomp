#pragma once
#include <cstdint>

#include <drm_fourcc.h>
#include <wayland-server-protocol.h>


namespace ccomp::gfx
{
    /**
     * @brief an information about pixel format
     */
    struct pixel_format_info
    {
        std::uint32_t drm_format { DRM_FORMAT_INVALID };

        /* same as the format if it has an alpha channel */
        std::uint32_t opaque_substitute { DRM_FORMAT_INVALID };

        std::uint32_t bytes_per_block { 0 };
        std::uint32_t block_width { 0 };
        std::uint32_t block_height { 0 };


        [[nodiscard]]
        static auto get_format_info(std::uint32_t format) noexcept
            -> pixel_format_info;


        [[nodiscard]]
        constexpr auto
        is_invalid() const noexcept -> bool
        {
            return drm_format == DRM_FORMAT_INVALID;
        }


        [[nodiscard]]
        auto get_pixels_per_block() const noexcept -> std::uint32_t;


        [[nodiscard]]
        auto get_min_stride(std::int32_t width) const noexcept -> std::int32_t;


        /**
         * @brief check whether a stride is large enough for
         *        a given pixel format and width
         */
        [[nodiscard]]
        auto check_stride(std::int32_t stride,
                          std::int32_t width) const noexcept -> bool;
    };


    [[nodiscard]]
    constexpr auto
    convert_wl_shm_format_to_drm(wl_shm_format format) noexcept -> std::uint32_t
    {
        switch (format)
        {
        case WL_SHM_FORMAT_XRGB8888: return DRM_FORMAT_XRGB8888;
        case WL_SHM_FORMAT_ARGB8888: return DRM_FORMAT_ARGB8888;
        default:                     return format;
        }
    }
}
