cmake_minimum_required(VERSION 3.1)
project(SymbolicAnalysisPass)
add_library(SymbolicAnalysisPass MODULE
    # List your source files here.
        SymbolicAnalysisPass.cpp SymbolicAnalysisPass.h)
target_link_libraries(SymbolicAnalysisPass IR_Record SymbolicTable)
# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(SymbolicAnalysisPass PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that.
# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
# LLVM is (typically) built with no C++ RTTI. We need to match that.
set_target_properties(SymbolicAnalysisPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )
if(APPLE)
    set_target_properties(SymbolicAnalysisPass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
set( EXECUTABLE_OUTPUT_PATH "build/")
