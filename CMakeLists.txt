cmake_minimum_required(VERSION 3.26)

project(visualizer CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_subdirectory(src/opengl)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_subdirectory(src/metal)
endif()

add_executable(${PROJECT_NAME} src/entry.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE metal-build)
