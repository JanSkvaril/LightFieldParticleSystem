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
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
int main()
{
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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ParticleEmitter ps(10);
    Model model("models/bird.obj");
    Camera camera;
    int density = 21;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    Generator generator(&model, density, 5000);
    generator.Generate();
    ps.AddTextureHandle(generator.CreateHandle());
    Skybox skybox;
    float time = 0.0f;
    TextReactangle rec;
    UiManager ui(window);
    // nanoguiWindow->center();
    std::cout << glGetString(GL_VERSION) << "\n";
    glEnable(GL_BLEND);

    while (!glfwWindowShouldClose(window))
    {
        const float speed = 0.05f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            camera.RotateAroundTarget(glm::vec2(speed, 0.0f));
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            camera.RotateAroundTarget(glm::vec2(-speed, 0.0f));
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            camera.RotateAroundTarget(glm::vec2(0.0f, speed));
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            camera.RotateAroundTarget(glm::vec2(0.0f, -speed));
        }

        ps.Update();
        time += 0.04f;

        processInput(window);
        // glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //  model.Draw(camera);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (ui.config.show_skybox)
        {
            skybox.Draw(camera);
        }
        if (!ui.config.show_light_field)
        {
            ps.Draw(camera, ui.config.density);
        }
        else
        {
            generator.Bind();
            rec.Draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
        }
        //(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        // Draw nanogui
        ui.Draw();
        ps.SetPactilesAmount(ui.config.amount_of_pacticles);
        ps.SetGravity(ui.config.gravity_strength, ui.config.gravity_direction);
        ps.ShouldShowBorders(ui.config.show_border);
        ps.SetSpeed(ui.config.particle_speed);
        ps.SetTimeToLive(ui.config.starting_time_to_live, ui.config.time_to_live_dispersion);
        generator.SetModelRotation(ui.config.model_rotation);
        generator.ChangeDensity(ui.config.density);
        // generator.ChangeResolution(ui.config.resolution);
        ps.GetRequiredAngles(generator.GetCacheTable(), camera, ui.config.density);
        generator.Generate();

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
