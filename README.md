#Weakest precondition analysis in the LLVM IR
Hope we can kick start from here

LLVM------------------>IR---------->Analysis--------->Transform----------->executable

Build with LLVM 3.7 and clang 3.7

Tried with LLVM 3.8 but label for block is missing
LLVM 3.7 keep each block name unique .


While building LLVM 3.7 make sure clang is install properly.

an error may occur at the end of the installation[ clang is not found in the directory] .

Just download the prebuild binaries from llvm official website and copy three file from the bin folder clang, clang++ and clang-format to the
bin directory of your build and run cmake build . --target install .

TODO:
 running the instnamer pass on IR, which will give an explicit name to everything, so we don't need to worry about finding implicit names.

To compile go to cmake-build-debug

$cmake ..
$make


If Z3 is installed everything will be fine.




