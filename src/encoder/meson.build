# Define encoder using function library()
hctree = library('encoder',
  sources: ['HCNode.hpp', 'HCTree.cpp', 'HCTree.hpp'], dependencies: [input_dep, output_dep])

inc = include_directories('.')

hctree_dep = declare_dependency(include_directories: inc,
  link_with: hctree)