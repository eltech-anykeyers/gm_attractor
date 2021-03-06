cmake_minimum_required(VERSION 3.0)

set(LIBS libs)
set(SOURCES src)
set(INCLUDE_DIRECTORIES include)
set(CMAKE_CXX_STANDARD 14)

project(AttractorViewer)

# Application
add_executable(${PROJECT_NAME}
    ${SOURCES}/main.cpp
    ${SOURCES}/glad.c
    ${SOURCES}/iglapp.cpp
    ${SOURCES}/attractorglapp.cpp
    ${SOURCES}/utils.cpp
    ${SOURCES}/shader.cpp
    ${SOURCES}/glmodel.cpp
    ${SOURCES}/attractormodel.cpp
    ${SOURCES}/fpsmanager.cpp
    ${SOURCES}/camera.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# GLM
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS}/glm)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${LIBS}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static" )
