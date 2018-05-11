cmake_minimum_required(VERSION 3.1)
project(IRPassSCC)
add_library(IRPassSCC MODULE
    # List your source files here.
        IRPassSCC.cpp  WP.cpp)
link_directories(/library)
target_link_libraries(IRPassSCC  IR_Record SymbolicTable Expression Z3Obj z3)
# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(IRPassSCC PRIVATE cxx_range_for cxx_auto_type)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# LLVM is (typically) built with no C++ RTTI. We need to match that.
# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
# LLVM is (typically) built with no C++ RTTI. We need to match that.
set_target_properties(IRPassSCC PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )
if(APPLE)
    set_target_properties(IRPassSCC PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
set( EXECUTABLE_OUTPUT_PATH "build/")
