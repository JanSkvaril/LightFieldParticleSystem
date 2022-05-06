cmake_minimum_required(VERSION 3.9)
project(LIGHT_FIELD_PS)
set(CMAKE_CXX_STANDARD 17)   

if (MSVC) 
    add_compile_options(/MP16) # set compile parallel
endif()



set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_SHARED  OFF CACHE BOOL " " FORCE)
set(NANOGUI_USE_GLAD  ON CACHE BOOL " " FORCE)


add_subdirectory(include/nanogui) 


set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")


add_definitions(${NANOGUI_EXTRA_DEFS})


include_directories(include/nanogui/include) 
include_directories(${NANOGUI_EXTRA_INCS}) 

add_subdirectory(include/glm)
add_subdirectory(include/assimp)

add_executable(light_field_ps_bp src/main.cpp
    src/shader.h
    src/shader.cpp
    src/particle.h
    src/particle.cpp
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
    src/text_rectangle.h
    src/text_rectangle.cpp
    src/camera.h
    src/camera.cpp
    src/ui_manager.h
    src/ui_manager.cpp
    src/ui_settings.h
    src/particle_params.h
    src/generator_params.h
    src/angle_cachetable.h
    src/angle_cachetable.cpp
    src/skybox.h
    src/skybox.cpp
    src/generator_store.h
    src/generator_store.cpp
    src/particle_leaf.h
    src/particle_leaf.cpp
    src/lightfield_ps_demo.h
    src/lightfield_ps_demo.cpp
    src/particle_random_model.h
    src/particle_random_model.cpp
    src/model_normals.h
    src/model_normals.cpp
    src/particle_simulator.h
    src/particle_simulator.cpp
    src/particle_standart3d.h
    src/particle_standart3d.cpp
    src/particle_starship.h
    src/particle_starship.cpp
    src/particle_halfchange.h
    src/particle_halfchange.cpp
    src/particle_flower.h
    src/particle_flower.cpp)


if (WIN32)
    target_compile_options(light_field_ps_bp PRIVATE /openmp)
endif (WIN32)

if (UNIX)
    target_compile_options(light_field_ps_bp PRIVATE -openmp)
endif (UNIX)


target_include_directories(light_field_ps_bp PRIVATE include/args)

target_link_libraries(light_field_ps_bp  nanogui ${NANOGUI_EXTRA_LIBS})
target_link_libraries(light_field_ps_bp glm)
target_link_libraries(light_field_ps_bp assimp)

# execute target
add_custom_target(run-ex1
    COMMAND light_field_ps_bp 
    DEPENDS light_field_ps_bp 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# copy assets folder target
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/
        )
add_dependencies(light_field_ps_bp copy_assets)
