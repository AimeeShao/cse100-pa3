subdir('bitStream')
subdir('encoder')

util = library('src', sources : ['FileUtils.hpp'], dependencies: [input_dep, output_dep, hctree_dep])
inc = include_directories('.')

util_dep = declare_dependency(include_directories : inc,
  link_with : util)

# Define compress_exe to output executable file named 
# compress.cpp.executable and define uncompress_exe to
# output executable file named uncompress.cpp.executable
compress_exe = executable('compress.cpp.executable',
    sources: ['compress.cpp'],
    dependencies: [input_dep, output_dep, hctree_dep, util_dep, cxxopts_dep],
    install: true)

uncompress_exe = executable('uncompress.cpp.executable', 
    sources: ['uncompress.cpp'],
    dependencies : [input_dep, output_dep, hctree_dep, util_dep, cxxopts_dep],
    install : true)
