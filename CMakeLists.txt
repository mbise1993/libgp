cmake_minimum_required(VERSION 3.10)
project(libgp VERSION 1.0.0)

# Hack to fix CMake generation 
# https://developercommunity.visualstudio.com/content/problem/249396/object-reference-not-set-to-an-instance-of-an-obje-17.html
add_definitions(-DFOO)

# Set global properties
enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add Cotire to speed up builds
include(cotire OPTIONAL)

# Setup Conan
include(${CMAKE_SOURCE_DIR}/cmake/conan/${CMAKE_BUILD_TYPE}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)
add_subdirectory(tests)
