subdir('libccomp')

if get_option('use-system-libccomp')
    libccomp = shared_library('libccomp', libccomp_src,
                              include_directories: libccomp_include,
                              version:             meson.project_version(),
                              soversion:           1,
                              dependencies:        deps,
                              install:             true)
else
    libccomp = static_library('libccomp', libccomp_src,
                              include_directories: libccomp_include,
                              dependencies:        deps)
endif

libccomp_dep = declare_dependency(include_directories: libccomp_include,
                                  link_with:           libccomp)