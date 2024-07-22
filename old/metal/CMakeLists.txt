find_package(SDL2 CONFIG REQUIRED)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp)
    file(
        DOWNLOAD https://developer.apple.com/metal/cpp/files/metal-cpp_macOS14.2_iOS17.2.zip 
        ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp_macOS14.2_iOS17.2.zip SHOW_PROGRESS
    )
    message("-- Downloaded metal-cpp")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar x metal-cpp_macOS14.2_iOS17.2.zip 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E rm metal-cpp_macOS14.2_iOS17.2.zip
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

add_library(metal-build STATIC renderer.cpp)

target_include_directories(metal-build PUBLIC metal-cpp)
target_include_directories(metal-build PUBLIC ${SDL2_INCLUDE_DIRS})

target_compile_features(metal-build PUBLIC cxx_std_17)

target_link_libraries(metal-build PUBLIC SDL2::SDL2
    "-framework Metal"
    "-framework MetalFX"
    "-framework Foundation"
    "-framework QuartzCore" 
)
