# OpenGL dependencies
find_package(GLEW CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(GLBuild STATIC 
	Application.cpp
	Shader.cpp
	Cube.cpp
)

target_compile_features(GLBuild PRIVATE cxx_std_17)

target_link_libraries(GLBuild PUBLIC GLEW::glew SDL2::SDL2 glm::glm-header-only imgui::imgui)
