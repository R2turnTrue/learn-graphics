cmake_minimum_required(VERSION 3.11)
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

project(learngraphics)
add_subdirectory(3rdparty)

add_executable (learngraphics src/main.cpp)

target_include_directories(learngraphics PUBLIC "src")
target_link_libraries(learngraphics PRIVATE glfw glad)