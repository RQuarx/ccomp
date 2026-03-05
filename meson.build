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
         dependency('gl'),
         dependency('libdrm'),
         dependency('entt', fallback: [ 'entt', 'entt_dep' ]) ]

args = [ '-DWL_HIDE_DEPRECATED',
         '-DCCOMP_VERSION="@0@"'.format(meson.project_version()),
         '-DCCOMP_IS_RELEASE=@0@'.format(get_option('buildtype')
                                             .contains('release')) ]

wl_mod = import('wayland')

xdg_shell_protocol = wl_mod.scan_xml(wl_mod.find_protocol('xdg-shell'),
                                     public: true,
                                     server: true)

subdir('app')
subdir('lib')

executable('ccomp', ccomp_src + xdg_shell_protocol,
           include_directories: [ ccomp_include_dir, libccomp_include ],
           dependencies:        deps,
           cpp_args:            args,
           link_with:           libccomp)