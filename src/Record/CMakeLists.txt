cmake_minimum_required(VERSION 3.1)
project(IR_Record)
add_library(IR_Record STATIC
        # List your source files here.
        IR_Record.cpp ../SymbolicAnalysisPass/SymbolicAnalysisPass.cpp ../SymbolicAnalysisPass/SymbolicAnalysisPass.h)
# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(IR_Record PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that.
# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
# LLVM is (typically) built with no C++ RTTI. We need to match that.
set_target_properties(IR_Record PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )
if(APPLE)
    set_target_properties(IR_Record PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)

