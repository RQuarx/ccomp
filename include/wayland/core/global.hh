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


    class impl_base
    {
    public:
        virtual ~impl_base() = default;
    };


    using interface_type = const struct wl_interface *const;


    template <typename T, typename... U>
    concept either = (std::same_as<T, U> || ...);


    template <typename T>
    concept global_type = std::derived_from<T, global_base> && requires {
        { T::interface } -> either<interface_type, interface_type &>;
        { T::version } -> either<const int, const int &>;
        { T::bind } -> std::convertible_to<wl_global_bind_func_t>;
    };


    template <typename T>
    concept impl_type = std::derived_from<T, impl_base> && requires {
        { T::get_impl() } -> std::convertible_to<const typename T::impl_type *>;
        { T::interface } -> either<interface_type, interface_type &>;
    };


    class global
    {
    public:
        template <class T_Display, global_type T_Global>
        global(T_Display                  &display,
               std::unique_ptr<T_Global> &&instance) noexcept
            : m_instance { std::move(instance) }, m_display { display },
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
