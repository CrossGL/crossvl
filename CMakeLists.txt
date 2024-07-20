cmake_minimum_required(VERSION 3.26)

project(visualizer CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(src/opengl)

add_executable(${PROJECT_NAME} src/main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2 imgui::imgui glm::glm-header-only GLBuild)