cmake_minimum_required(VERSION 3.16)
project(ConcertStage VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default: build the included GLFW source; no package manager or download at configure time.
option(CONCERT_USE_SYSTEM_GLFW "Use a preinstalled GLFW instead of the included source" OFF)
if(CONCERT_USE_SYSTEM_GLFW)
    find_package(glfw3 3.3 REQUIRED)
else()
    set(GLFW_LIBRARY_TYPE STATIC CACHE STRING "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)
endif()

add_executable(concert_stage
    src/main.cpp src/GL.cpp src/Input.cpp src/Camera.cpp src/Shader.cpp
    src/Mesh.cpp src/Renderer.cpp src/Hud.cpp
    src/scene/Stage.cpp src/scene/LightShow.cpp src/scene/Crowd.cpp src/scene/LedScreen.cpp)
target_include_directories(concert_stage PRIVATE include)
target_link_libraries(concert_stage PRIVATE glfw)
target_compile_definitions(concert_stage PRIVATE CONCERT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
if(MSVC)
    target_compile_options(concert_stage PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(concert_stage PRIVATE -Wall -Wextra -Wpedantic)
endif()
set_property(TARGET concert_stage PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_custom_command(TARGET concert_stage POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:concert_stage>/shaders")

include(CTest)
if(BUILD_TESTING)
    add_executable(math_tests tests/math_tests.cpp)
    target_include_directories(math_tests PRIVATE include)
    add_test(NAME transforms_and_coordinates COMMAND math_tests)
endif()
# Shader files remain editable after the first build, including in multi-config builds.
add_custom_target(sync_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:concert_stage>/shaders")
add_dependencies(sync_shaders concert_stage)
