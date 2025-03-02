cmake_minimum_required(VERSION 3.11)
include(FetchContent)
include(ae2f.cmake)

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
target_link_libraries(learngraphics PRIVATE glfw glad stb_image glm::glm)