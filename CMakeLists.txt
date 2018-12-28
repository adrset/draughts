cmake_minimum_required(VERSION 3.1)

project(WARCABY)

set (CMAKE_CXX_STANDARD 11)

include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/src/include")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/glfw/include")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/glm")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/")
include_directories ("${PROJECT_SOURCE_DIR}/src/lib/stb")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/build )

add_subdirectory (src/lib/beef2d) 
add_subdirectory (src/network) 
add_subdirectory (src/lib/glfw)
add_subdirectory (src/lib/glad)
add_subdirectory (src/lib/stb)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)

add_executable (Warcaby src/Game.cpp src/main.cpp)

target_link_libraries (Warcaby beef2d network glfw glad stb)
