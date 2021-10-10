cmake_minimum_required(VERSION 3.9)


project(LIGHT_FIELD_PS)


set(CMAKE_CXX_STANDARD 17)     

add_executable(light_field_ps_bp src/main.cpp)


set(OpenGL_GL_PREFERENCE GLVND)
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(light_field_ps_bp glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(light_field_ps_bp OpenGL::GL)

add_library(glad include/glad/glad.h src/glad.c)
target_link_libraries(glad dl)
target_include_directories(glad PUBLIC include/)
target_link_libraries(light_field_ps_bp glad)
    
add_custom_target(run-ex1
    COMMAND light_field_ps_bp 
    DEPENDS light_field_ps_bp 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

