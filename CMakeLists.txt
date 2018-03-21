cmake_minimum_required(VERSION 2.8)

set(LIBS libs)
set(SOURCES src)
set(INCLUDE_DIRECTORIES include)
set(CMAKE_CXX_STANDARD 11)

project(AttractorViewer)

# Application
add_executable(${PROJECT_NAME} ${SOURCES}/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${LIBS}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
