project('ccomp', 'cpp', 'c',
        version:       '0.0.1',
        meson_version: '>=1.8.0',
        license:       'GPL-3.0-or-later',
        license_files:   [ 'COPYING' ],
        default_options: [ 'cpp_std=c++23',
                           'warning_level=3' ])

deps = [ dependency('wayland-protocols'),
         dependency('wayland-server'),
         dependency('cairomm-1.16'),
         dependency('entt', fallback: [ 'entt', 'entt_dep' ]) ]

wl_mod = import('wayland')

# core_xml   = wl_mod.find_protocol('wayland')
# core_proto = wl_mod.scan_xml(core_xml,
#                              public: true,
#                              server: true)

xml       = wl_mod.find_protocol('xdg-shell')
xdg_proto = wl_mod.scan_xml(xml,
                            public: true,
                            server: true)

subdir('src')
executable('ccomp', ccomp_src + xdg_proto,
           include_directories: include_directories('include'),
           dependencies:        deps,
           cpp_args: [ '-DWL_HIDE_DEPRECATED' ])