cmake_minimum_required(VERSION 3.11)
include(FetchContent)
include(ae2f.cmake)

set(CMAKE_BUILD_TYPE "Release")
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(assimp
GIT_REPOSITORY https://github.com/assimp/assimp.git
GIT_TAG master)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fexec-charset=UTF-8")
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
FetchContent_MakeAvailable(assimp)

ae2f_CoreLibFetch(
    glfw
    glfw
    3.4
)

ae2f_CoreLibFetch(
    g-truc
    glm
    master
)

project(learngraphics)
add_subdirectory(3rdparty)

file(GLOB_RECURSE VAR "${PROJECT_SOURCE_DIR}/src/*.c*")

add_executable (learngraphics ${VAR})

target_include_directories(learngraphics PUBLIC "src")
target_link_libraries(learngraphics PRIVATE glfw glad stb_image glm::glm assimp)