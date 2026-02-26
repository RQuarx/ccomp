#pragma once
#include <string>

#include <wayland-server.h>

#include "math/size.hh"

#include "cairomm/types.h"


namespace ccomp::wl::util
{
    template <typename T>
    [[nodiscard]]
    static constexpr auto
    get_user_data(wl_resource *resource) noexcept -> T *
    {
        return static_cast<T *>(wl_resource_get_user_data(resource));
    }


    template <typename T>
    [[nodiscard]]
    static constexpr auto
    get_user_data(void *data) noexcept -> T *
    {
        return static_cast<T *>(data);
    }


    struct credential
    {
        pid_t pid;
        uid_t uid;
        gid_t gid;
    };

#define GET_CREDENTIAL(c) &(c).pid, &(c).uid, &(c).gid


    [[nodiscard]]
    static constexpr auto
    apply_inverse_transform(const Cairo::RectangleInt &rect,
                            math::size                 buffer_size,
                            std::int32_t               transform) noexcept
        -> Cairo::RectangleInt
    {
        Cairo::RectangleInt out;

        int rotation { transform & 0x3 }; /* lower 2 bits = rotation */

        switch (rotation)
        {
        case WL_OUTPUT_TRANSFORM_NORMAL: out = rect; break;

        case WL_OUTPUT_TRANSFORM_90:
            out.x = buffer_size.h - (rect.y + rect.height);
            out.y = rect.x;
            out.width = rect.height;
            out.height = rect.width;
            break;

        case WL_OUTPUT_TRANSFORM_180:
            out.x = buffer_size.w - (rect.x + rect.width);
            out.y = buffer_size.h - (rect.y + rect.height);
            out.width = rect.width;
            out.height = rect.height;
            break;

        case WL_OUTPUT_TRANSFORM_270:
            out.x = rect.y;
            out.y = buffer_size.w - (rect.x + rect.width);
            out.width = rect.height;
            out.height = rect.width;
            break;

        default: out = rect; break;
        }

        bool flip { (transform & 0x4) != 0 };
        if (flip) out.x = buffer_size.w - (out.x + out.width);

        return out;
    }


    void destroy_client_resource(wl_client *, wl_resource *resource) noexcept;

    [[nodiscard]]
    auto get_client_tag(const wl_client *client) noexcept -> std::string;
}
