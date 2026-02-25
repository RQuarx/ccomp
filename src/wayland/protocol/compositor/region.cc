#include "logger.hh"
#include "wayland/protocol/compositor.hh"
#include "wayland/util.hh"

using ccomp::wl::protocol::_impl::region;

#define DOMAIN "ccomp::wl::protocol::compositor"


void
region::on_destroy(wl_resource *resource) noexcept
{
    auto *self { util::get_user_data<data_type>(resource) };
    delete self;
}


void
region::destroy(wl_client * /* client */, wl_resource *resource) noexcept
{
    wl_resource_destroy(resource);
}


void
region::add(wl_client * /* client */,
            wl_resource *resource,
            std::int32_t x,
            std::int32_t y,
            std::int32_t width,
            std::int32_t height) noexcept
{
    auto *self { util::get_user_data<data_type>(resource) };
    self->emplace_back(x, y, width, height);
}


void
region::subtract(wl_client   *client,
                 wl_resource *resource,
                 std::int32_t x,
                 std::int32_t y,
                 std::int32_t width,
                 std::int32_t height) noexcept
{
    auto *self { util::get_user_data<data_type>(resource) };
    auto  it {
        std::ranges::find_if(
            *self,
            [a { math::rect { .x = x, .y = y, .w = width, .h = height } }
             ](
                const math::rect &b) -> bool
            { return (a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h);                                                      }
             )
    };

    if (it != self->end())
    {
        self->erase(it);
        return;
    }

    util::credential creds;
    wl_client_get_credentials(client, GET_CREDENTIAL(creds));

    logger[log_level::warn, DOMAIN](
        "client[{}-{}-{}] attempted to subtract a non-existent rectangle",
        creds.pid, creds.uid, creds.gid);
}
