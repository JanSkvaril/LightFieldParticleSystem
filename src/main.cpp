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
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void printFPS()
{
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps;
    fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{1})
    {
        oldTime = std::chrono::high_resolution_clock::now();
        std::cout << "FPS: " << fps << std::endl;
        fps = 0;
    }
}

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
int main(int argc, char **argv)
{
    args::ArgumentParser parser("This is demo application for Lightfield Particle System by Jan Skvaril", "All options are optional, max 1 scene can be selected");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "Flags:", args::Group::Validators::DontCare);
    args::Flag opt_no_vsync(group, "novsync", "Set unlimited PFS", {'n', "novsync"});
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
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light Field Particles", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    if (opt_no_vsync)
    {
        glfwSwapInterval(0);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float time = 0.0f;
    LightFieldPsDemo lfps({SCR_WIDTH, SCR_HEIGHT});

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

        lfps.SetPresetHalfChange();
    }

    if (opt_particles)
    {
        lfps.particles.SetPactilesAmount(opt_particles.Get());
    }
    if (opt_resolution)
    {
        lfps.generator_store.SetResolution(opt_resolution.Get());
        lfps.particles.AddTextureHandle(lfps.generator_store);
    }
    if (opt_scene_benchmark)
    {
        lfps.particles.SimulateSteps(10000);
    }
    UiManager ui(window);
    ui.AddLFPS(&lfps);
    // std::cout << glGetString(GL_VERSION) << "\n";
    glEnable(GL_BLEND);
    TextReactangle rec;

    while (!glfwWindowShouldClose(window))
    {
        if (opt_print_fps)
        {
            printFPS();
        }
        ui.HandleCameraControls(lfps.camera);

        lfps.Update();
        // bench.Update();
        time += 0.04f;

        processInput(window);
        glEnable(GL_BLEND);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        lfps.Generate();
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        if (ui.config.show_skybox)
        {
            lfps.DrawSkybox();
        }
        if (!ui.config.show_light_field)
        {
            lfps.Draw();
            //  bench.Draw(lfps.camera);
        }
        else
        {
            lfps.generator_store.Generators.front()->Bind();
            rec.Draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
        }

        ui.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //  glDeleteVertexArrays(1, &VAO);
    //   glDeleteBuffers(1, &VBO);
    //   glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
