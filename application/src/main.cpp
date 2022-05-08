// Light field particle system demo
// This application was created as part of Bachelor's thesis at VUT FIT Brno
/*              Abstract
The goal of this thesis is to propose and implement a method combining light field and
particle effects. Particles are rendered as billboards and their textures are created with
synthetic light field, which is dynamically generated at runtime. Particles can also use
several of these textures at the same time. The thesis also includes several implemented
scenes demonstrating various functionality and measurements comparing proposed method
with standard 3D rendering techniques.
*/
// Author: Jan Škvařil
// 2022
// LICENCE: MIT

// This application required GPU supporting bindless textures (many Intel integrated GPUs do
// not support this functionality)

#define GLFW_INCLUDE_GLEXT
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "shader.h"
#include "particle_emitter.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "generator.h"
#include "generator_texture.h"
#include "text_rectangle.h"
#include <nanogui/nanogui.h>

#include "ui_manager.h"
#include <vector>
#include <string>
#include "skybox.h"
#include "generator_store.h"
#include "memory"
#include "particle_leaf.h"
#include "lightfield_ps_demo.h"
#include "particle_standart3d.h"
#include <chrono>
#include "args.hxx"
#include <exception>
void processInput(GLFWwindow *window);

#ifndef __linux__ 
// turn off optimus!
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
#endif
// print fps to stdout
void printFPS()
{
    #ifndef __linux__ 
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps;
    fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{1})
    {
        oldTime = std::chrono::high_resolution_clock::now();
        std::cout << "FPS: " << fps << std::endl;
        fps = 0;
    }
    #endif
}

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int main(int argc, char **argv)
{
    // ===== arguments ====
    args::ArgumentParser parser("This is demo application for Lightfield Particle System by Jan Skvaril", "All options are optional, max 1 scene can be selected");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "Flags:", args::Group::Validators::DontCare);
    args::Flag opt_no_vsync(group, "novsync", "Set unlimited PFS", {'n', "novsync"});
    args::Flag opt_cam_rot(group, "rotate camera", "Camera will rotate", {'c', "camrot"});
    args::Flag opt_print_fps(group, "print fps", "Should print FPS to STDOUT", {'f', "fpsprint"});
    args::Group group2(parser, "Parameters:", args::Group::Validators::DontCare);
    args::ValueFlag<int> opt_particles(group2, "particles", "Starting amount of particles", {'p', "particles"});
    args::ValueFlag<int> opt_resolution(group2, "resolution", "Set light field texture resolution e.g.: -r=10000 is texture 1000x1000 px", {'r', "resolution"});
    args::Group group3(parser, "Scene:", args::Group::Validators::AtMostOne);
    args::Flag opt_scene_basic(group3, "Basic", "Basic scene", {"sbasic"});
    args::Flag opt_scene_benchmark(group3, "Benchmark", "Special scene for benchmark", {"sbench"});
    args::Flag opt_scene_benchmarkC(group3, "Benchmark", "Special scene for benchmark with very complex model", {"sbenchc"});
    args::Flag opt_scene_real_light(group3, "Real light", "Scene with realtime light", {"slight"});
    args::Flag opt_scene_standard3d(group3, "Standard 3D", "Particles are rendered with normal 3D methods", {"s3d"});
    args::Flag opt_scene_standard3dC(group3, "Standard 3D", "Particles are rendered with normal 3D methods with very complex model", {"s3dc"});
    args::Flag opt_scene_disco(group3, "Disco", "Disco scene, with pariodical light field redraw", {"disco"});
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (std::exception e)
    {
        std::cout << "Argument error, -h for help\n";
        return 1;
    }

    // ===== glfw setup ====
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ===== window ====
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light Field Particles", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, nullptr);

    // ===== Load opengl function loader ====
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ===== prepare scene from arguments ====
    if (opt_no_vsync)
    {
        glfwSwapInterval(0); // unlimited FPS
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float time = 0.0f;
    std::srand(0); // same seed every time

    // create demo object
    LightFieldPsDemo lfps({SCR_WIDTH, SCR_HEIGHT});

    // select scene
    if (opt_scene_benchmark)
    {
        lfps.SetPresetBenchmark();
    }
    else if (opt_scene_real_light)
    {
        lfps.SetPresetRealLight();
    }
    else if (opt_scene_benchmarkC)
    {
        lfps.SetPresetBenchmarkComplex();
    }
    else if (opt_scene_benchmarkC)
    {
        lfps.SetPresetBenchmarkComplex();
    }
    else if (opt_scene_disco)
    {
        lfps.SetPresetDiscoBench();
    }
    else if (opt_scene_standard3dC)
    {
        int part = 1000;
        if (opt_particles)
        {
            part = opt_particles.Get();
        }
        lfps.SetPresetNoLightfieldComplex(part);
    }
    else if (opt_scene_standard3d)
    {
        int part = 1000;
        if (opt_particles)
        {
            part = opt_particles.Get();
        }
        lfps.SetPresetNoLightfield(part);
    }
    else
    {

        lfps.SetPresetBasic();
    }
    // parameters from CLI
    if (opt_particles)
    {
        lfps.particles.SetPactilesAmount(opt_particles.Get());
    }
    if (opt_resolution)
    {
        lfps.generator_store.SetResolution(opt_resolution.Get());
        lfps.particles.AddTextureHandle(lfps.generator_store);
    }

    // ===== GUI setup ====
    UiManager ui(window);
    ui.AddLFPS(&lfps);
    if (opt_no_vsync)
    {
        lfps.particles.Reset();
        lfps.particles.SimulateSteps(1000);
        ui.ShouldDisplayui(false);
    }
    if (opt_cam_rot)
    {
        lfps.ShouldCameraRotate(true);
    }

    glEnable(GL_BLEND);
    TextReactangle rec; // rectangle for displaying light field texture

    // ===== MAIN RENDER LOOP ====
    while (!glfwWindowShouldClose(window))
    {
        // print FPS, for benchmarks
        if (opt_print_fps)
        {
            printFPS();
        }

        // update camera controlls
        ui.HandleCameraControls(lfps.camera);

        // simulalate
        lfps.Update();

        // clear buffer
        processInput(window);
        glEnable(GL_BLEND);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // generate light field textures
        lfps.Generate();
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        // Draw skybox if allowed
        if (ui.config.show_skybox)
        {
            lfps.DrawSkybox();
        }
        if (!ui.config.show_light_field)
        {
            // Draw light field particle system
            lfps.Draw();
        }
        else
        {
            // draw just light field texture instead
            lfps.generator_store.Generators.front()->Bind();
            rec.Draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
        }
        // draw ui
        ui.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    lfps.generator_store.Clear(); // dispose textures
    glfwTerminate();
    return 0;
}

// process keyboard input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
