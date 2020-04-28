#
# Root project
#

project('irbis', 'cpp',
    version: '0.0.1',
    license: 'MIT',
    meson_version: '>= 0.45.0',
    default_options : ['cpp_std=c++11',
        'buildtype=release'])

cpp = meson.get_compiler('cpp') # C++ compiler for further usage

commonInclude = [ include_directories('include'),
        include_directories('libs/irbis/include'),
        include_directories('third')
    ]

libirbis_dep = [] # dummy declaration, will be redefined later

subdir('libs')
subdir('apps')
subdir('tests')
