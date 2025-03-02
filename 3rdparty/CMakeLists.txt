add_library(glad STATIC ${CMAKE_SOURCE_DIR}/3rdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty/glad/include)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/stb_image)