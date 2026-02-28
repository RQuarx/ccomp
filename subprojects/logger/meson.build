project('logger', 'cpp',
        version: '1',
        license: 'GPL-3.0-or-later',
        default_options: [ 'cpp_std=c++23' ])


logger_src = files('logger.cc')
logger_inc = files('logger.hh')


lib = static_library('logger', logger_src)

logger_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with:           lib)