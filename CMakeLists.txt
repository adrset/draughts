cmake_minimum_required(VERSION 3.0)

project(WARCABY)

include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/src/include")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/glfw")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/glfw/include")

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/build )

add_subdirectory (src/GameEngine) 
add_subdirectory (src/lib/glfw)
add_subdirectory (src/lib/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable (Warcaby src/Game.cpp src/main.cpp)

target_link_libraries (Warcaby GameEngine glfw glad)
