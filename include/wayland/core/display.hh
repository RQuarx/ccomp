#pragma once
#include <memory>
#include <optional>
#include <string_view>
#include <typeindex>
#include <unordered_map>

#include <wayland-server-core.h>

#include "wayland/core/event_loop.hh"
#include "wayland/core/global.hh"


namespace ccomp::wl::core
{
    class display
    {
    public:
        static auto create() noexcept -> std::optional<display>;
        ~display();


        display(const display &)                     = delete;
        auto operator=(const display &) -> display & = delete;

        display(display &&other) noexcept;
        auto operator=(display &&other) noexcept -> display &;


        auto add_socket(const char *name) noexcept
            -> std::optional<std::string_view>;


        void run() noexcept;


        [[nodiscard]] auto raw() noexcept -> wl_display *;
        [[nodiscard]] auto get_event_loop() noexcept -> event_loop_ref;


        template <global_type T_Global, typename... T_ConstructArgs>
        void
        create_global(T_ConstructArgs &&...args) noexcept
        {
            auto obj { std::make_unique<T_Global>(
                std::forward<T_ConstructArgs>(args)...) };

            m_global[typeid(T_Global)]
                = std::make_unique<global>(*this, std::move(obj));
        }

    private:
        wl_display *m_display;

        std::unordered_map<std::type_index, std::unique_ptr<global>> m_global;

        display(wl_display *display) noexcept;
    };
}
