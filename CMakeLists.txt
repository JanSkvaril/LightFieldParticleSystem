cmake_minimum_required(VERSION 3.9)
project(LIGHT_FIELD_PS)
set(CMAKE_CXX_STANDARD 17)   
set(CMAKE_CXX_FLAGS_RELEASE "/MT")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")  
add_executable(light_field_ps_bp src/main.cpp
    src/shader.h
    src/shader.cpp
    src/particle.h
    src/particle_emitter.h
    src/particle_emitter.cpp
    src/texture.h
    src/texture.cpp
    src/model.h
    src/model.cpp
    src/model_vertex.h
    src/generator.h
    src/generator.cpp
    src/generator_texture.cpp
    src/generator_texture.h
    src/rectangle.h
    src/rectangle.cpp
    src/camera.h
    src/camera.cpp)

if(MSVC)
    target_compile_options(light_field_ps_bp PRIVATE "/MP")
endif()
# open gl
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(include/glfw)

set(OpenGL_GL_PREFERENCE GLVND)
#find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(light_field_ps_bp glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(light_field_ps_bp OpenGL::GL)
# glad
add_library(glad include/glad/glad.h src/glad.c)
#target_link_libraries(glad dl)
target_include_directories(glad PUBLIC include/)
target_link_libraries(light_field_ps_bp glad)

add_subdirectory(include/glm)
target_link_libraries(light_field_ps_bp glm)

add_subdirectory(include/assimp)
if(MSVC)
    target_compile_options(assimp PRIVATE "/MP")
endif()
target_link_libraries(light_field_ps_bp assimp)

# stb image
add_library(stb_img INTERFACE)
target_include_directories(stb_img INTERFACE include/stb_image/stb_image.h)
target_link_libraries(light_field_ps_bp stb_img)

# run target
add_custom_target(run-ex1
    COMMAND light_field_ps_bp 
    DEPENDS light_field_ps_bp 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)


add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/
        )
add_dependencies(light_field_ps_bp copy_assets)
