test_BitInputStream_exe = executable('test_BitInputStream.cpp.executable', 
    sources: ['test_BitInputStream.cpp'], 
    dependencies : [input_dep, util_dep, gtest_dep])
test('my BitInputStream test', test_BitInputStream_exe)

test_BitOutputStream_exe = executable('test_BitOutputStream.cpp.executable', 
    sources: ['test_BitOutputStream.cpp'], 
    dependencies : [output_dep, util_dep, gtest_dep])
test('my BitInputStream test', test_BitInputStream_exe)

test_HCTree_exe = executable('test_HCTree.cpp.executable', 
    sources: ['test_HCTree.cpp'], 
    dependencies : [input_dep, output_dep, hctree_dep, util_dep, gtest_dep])
test('my HCTree test', test_HCTree_exe)