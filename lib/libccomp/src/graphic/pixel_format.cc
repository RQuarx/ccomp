#include <algorithm>
#include <array>
#include <cmath>

#include "graphic/pixel_format.hh"

using ccomp::gfx::pixel_format_info;

namespace
{
    /* clang-format off */
    constexpr const std::array<pixel_format_info, 42> pixel_format_informations
    {{
        {
            .drm_format      = DRM_FORMAT_XRGB8888,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_ARGB8888,
            .opaque_substitute = DRM_FORMAT_XRGB8888,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_XBGR8888,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_ABGR8888,
            .opaque_substitute = DRM_FORMAT_XBGR8888,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_RGBX8888,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_RGBA8888,
            .opaque_substitute = DRM_FORMAT_RGBX8888,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_BGRX8888,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_BGRA8888,
            .opaque_substitute = DRM_FORMAT_BGRX8888,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_R8,
            .bytes_per_block = 1,
        }, {
            .drm_format      = DRM_FORMAT_R16F,
            .bytes_per_block = 2,
        }, {
            .drm_format      = DRM_FORMAT_R32F,
            .bytes_per_block = 4,
        }, {
            .drm_format      = DRM_FORMAT_GR88,
            .bytes_per_block = 2,
        }, {
            .drm_format      = DRM_FORMAT_GR1616F,
            .bytes_per_block = 4,
        }, {
            .drm_format      = DRM_FORMAT_GR3232F,
            .bytes_per_block = 8,
        }, {
            .drm_format      = DRM_FORMAT_RGB888,
            .bytes_per_block = 3,
        }, {
            .drm_format      = DRM_FORMAT_BGR888,
            .bytes_per_block = 3,
        }, {
            .drm_format      = DRM_FORMAT_BGR161616,
            .bytes_per_block = 6,
        }, {
            .drm_format      = DRM_FORMAT_BGR161616F,
            .bytes_per_block = 6,
        }, {
            .drm_format      = DRM_FORMAT_BGR323232F,
            .bytes_per_block = 12,
        }, {
            .drm_format      = DRM_FORMAT_RGBX4444,
            .bytes_per_block = 2,
        }, {
            .drm_format        = DRM_FORMAT_RGBA4444,
            .opaque_substitute = DRM_FORMAT_RGBX4444,
            .bytes_per_block   = 2,
        }, {
            .drm_format      = DRM_FORMAT_BGRX4444,
            .bytes_per_block = 2,
        }, {
            .drm_format        = DRM_FORMAT_BGRA4444,
            .opaque_substitute = DRM_FORMAT_BGRX4444,
            .bytes_per_block   = 2,
        }, {
            .drm_format      = DRM_FORMAT_RGBX5551,
            .bytes_per_block = 2,
        }, {
            .drm_format        = DRM_FORMAT_RGBA5551,
            .opaque_substitute = DRM_FORMAT_RGBX5551,
            .bytes_per_block   = 2,
        }, {
            .drm_format      = DRM_FORMAT_BGRX5551,
            .bytes_per_block = 2,
        }, {
            .drm_format        = DRM_FORMAT_BGRA5551,
            .opaque_substitute = DRM_FORMAT_BGRX5551,
            .bytes_per_block   = 2,
        }, {
            .drm_format      = DRM_FORMAT_XRGB1555,
            .bytes_per_block = 2,
        }, {
            .drm_format        = DRM_FORMAT_ARGB1555,
            .opaque_substitute = DRM_FORMAT_XRGB1555,
            .bytes_per_block   = 2,
        }, {
            .drm_format      = DRM_FORMAT_RGB565,
            .bytes_per_block = 2,
        }, {
            .drm_format      = DRM_FORMAT_BGR565,
            .bytes_per_block = 2,
        }, {
            .drm_format      = DRM_FORMAT_XRGB2101010,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_ARGB2101010,
            .opaque_substitute = DRM_FORMAT_XRGB2101010,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_XBGR2101010,
            .bytes_per_block = 4,
        }, {
            .drm_format        = DRM_FORMAT_ABGR2101010,
            .opaque_substitute = DRM_FORMAT_XBGR2101010,
            .bytes_per_block   = 4,
        }, {
            .drm_format      = DRM_FORMAT_XBGR16161616F,
            .bytes_per_block = 8,
        }, {
            .drm_format        = DRM_FORMAT_ABGR16161616F,
            .opaque_substitute = DRM_FORMAT_XBGR16161616F,
            .bytes_per_block   = 8,
        }, {
            .drm_format      = DRM_FORMAT_ABGR32323232F,
            .bytes_per_block = 16,
        }, {
            .drm_format      = DRM_FORMAT_XBGR16161616,
            .bytes_per_block = 8,
        }, {
            .drm_format        = DRM_FORMAT_ABGR16161616,
            .opaque_substitute = DRM_FORMAT_XBGR16161616,
            .bytes_per_block   = 8,
        }, {
            .drm_format      = DRM_FORMAT_YVYU,
            .bytes_per_block = 4,
            .block_width     = 2,
            .block_height    = 1,
        }, {
            .drm_format      = DRM_FORMAT_VYUY,
            .bytes_per_block = 4,
            .block_width     = 2,
            .block_height    = 1,
        },
    }};
    /* clang-format on */

    constexpr const std::array<std::uint32_t, 23> opaque_pixel_formats {
        DRM_FORMAT_XRGB8888,     DRM_FORMAT_XBGR8888,
        DRM_FORMAT_RGBX8888,     DRM_FORMAT_BGRX8888,
        DRM_FORMAT_R8,           DRM_FORMAT_GR88,
        DRM_FORMAT_RGB888,       DRM_FORMAT_BGR888,
        DRM_FORMAT_RGBX4444,     DRM_FORMAT_BGRX4444,
        DRM_FORMAT_RGBX5551,     DRM_FORMAT_BGRX5551,
        DRM_FORMAT_XRGB1555,     DRM_FORMAT_RGB565,
        DRM_FORMAT_BGR565,       DRM_FORMAT_XRGB2101010,
        DRM_FORMAT_XBGR2101010,  DRM_FORMAT_XBGR16161616F,
        DRM_FORMAT_XBGR16161616, DRM_FORMAT_YVYU,
        DRM_FORMAT_VYUY,         DRM_FORMAT_NV12,
        DRM_FORMAT_P010,
    };
}


auto
pixel_format_info::get_format_info(std::uint32_t format) noexcept
    -> pixel_format_info
{
    const auto *it { std::ranges::find_if(
        pixel_format_informations,
        [format](const pixel_format_info &info) -> bool
        { return format == info.drm_format; }) };

    return it != pixel_format_informations.end() ? *it : pixel_format_info {};
}


auto
pixel_format_info::get_pixels_per_block() const noexcept -> std::uint32_t
{
    std::uint32_t pixels { block_width * block_height };
    return pixels > 0 ? pixels : 1;
}


auto
pixel_format_info::get_min_stride(std::int32_t width) const noexcept
    -> std::int32_t
{
    auto pixels_per_block { get_pixels_per_block() };

    if (static_cast<std::uint32_t>(width)
        > std::numeric_limits<std::int32_t>::max() / bytes_per_block)
        return std::numeric_limits<std::int32_t>::min();

    return std::ceil((width * bytes_per_block) / pixels_per_block);
}


auto
pixel_format_info::check_stride(std::int32_t stride,
                                std::int32_t width) const noexcept -> bool
{
    if (stride % bytes_per_block != 0) return false;

    if (auto min_stride { get_min_stride(width) };
        min_stride <= 0 || stride < min_stride)
        return false;

    return true;
}
