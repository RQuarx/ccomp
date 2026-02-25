#pragma once
#include <memory>

#include <wayland-server-core.h>


namespace ccomp::wl::core
{
    class global_base
    {
    public:
        virtual ~global_base() = default;
    };


    using interface_type = const struct wl_interface *const;


    template <typename T>
    concept global_type = std::derived_from<T, global_base> && requires {
        { T::interface } -> std::same_as<interface_type>;
        { T::version } -> std::same_as<int>;
        { T::bind } -> std::same_as<wl_global_bind_func_t>;
    };


    class global
    {
    public:
        template <global_type T_Global, class T_Display, typename... T_Args>
        global(T_Display &display, T_Args &&...args) noexcept
            : m_instance { std::make_unique<T_Global>(
                  std::forward<T_Args>(args)...) },
              m_display { display },
              m_global { wl_global_create(display.raw(),
                                          T_Global::interface,
                                          T_Global::version,
                                          m_instance.get(),
                                          T_Global::bind) },
              m_interface { T_Global::interface },
              m_version { T_Global::version }
        {
        }

        ~global();

        global(const global &)                     = delete;
        auto operator=(const global &) -> global & = delete;

        global(global &&other) noexcept;
        auto operator=(global &&other) noexcept -> global &;


        [[nodiscard]] auto get_display() const noexcept -> class display &;
        [[nodiscard]] auto get_version() const noexcept -> int;
        [[nodiscard]] auto get_instance() const noexcept -> global_base *;
        [[nodiscard]] auto get_interface() const noexcept -> const
            struct wl_interface *;

    private:
        std::unique_ptr<global_base> m_instance;

        class display &m_display;
        wl_global     *m_global { nullptr };


        const wl_interface *m_interface { nullptr };
        int                 m_version { 0 };
    };
}
