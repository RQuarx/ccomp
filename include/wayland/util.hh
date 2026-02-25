#pragma once
#include <wayland-server.h>


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
}