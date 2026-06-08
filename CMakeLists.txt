cmake_minimum_required(VERSION 3.16)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(include/glad/cmake)

glad_add_library(glad_gl_core_33 STATIC API gl:core=3.3)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/gladsources/glad_gl_core_33/include
)

target_link_libraries(${PROJECT_NAME}
    glad_gl_core_33
    OpenGL::GL
    glfw
    dl
)