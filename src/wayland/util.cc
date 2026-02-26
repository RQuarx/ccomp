#include <format>
#include "wayland/util.hh"

using namespace ccomp::wl;


void
util::destroy_client_resource(wl_client * /* unused */,
                              wl_resource *resource) noexcept
{
    wl_resource_destroy(resource);
}


auto
util::get_client_tag(const wl_client *client) noexcept -> std::string
{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    wl_client_get_credentials(client, &pid, &uid, &gid);

    return std::format("client[{}-{}-{}]", pid, uid, gid);
}
