cmake_minimum_required(VERSION 2.8.9)
project (ppu)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/bgfx/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/bgfx/examples)
include_directories(${CMAKE_SOURCE_DIR}/lib/bx/include)

link_directories(${CMAKE_SOURCE_DIR}/lib/bgfx/.build/linux64_gcc/bin)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(ppu ${sources})

# add bgfx lib
target_link_libraries(ppu example-commonRelease bgfxRelease bimgRelease bxRelease GL GLU X11 dl)


find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(ppu PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(ppu "${CMAKE_THREAD_LIBS_INIT}")
endif()

