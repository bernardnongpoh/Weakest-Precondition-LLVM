cmake_minimum_required(VERSION 3.1)
project(SymbolicTable)
add_library(SymbolicTable SHARED
        # List your source files here.
        SymbolicTable.cpp)
# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_link_libraries(SymbolicTable  IR_Record )
target_compile_features(SymbolicTable PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that.
# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
# LLVM is (typically) built with no C++ RTTI. We need to match that.
set_target_properties(SymbolicTable PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )
if(APPLE)
    set_target_properties(SymbolicTable PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)

