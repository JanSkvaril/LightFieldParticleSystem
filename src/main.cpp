#define STB_IMAGE_IMPLEMENTATION
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    int a = 10;
    ParticleEmitter ps(a);
    Model model("models/bird.obj");
    Camera camera;
    const int density = 21;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    Generator generator(&model, density, 5000);
    generator.Generate();
    //  GeneratorTexture gt(&model, 1000, 1000);

    float time = 0.0f;
    TextReactangle rec;

    nanogui::Screen screen = nanogui::Screen();
    screen.initialize(window, true);
    nanogui::FormHelper *gui = new nanogui::FormHelper(&screen);
    nanogui::ref<nanogui::Window> nanoguiWindow = gui->add_window(nanogui::Vector2i(10, 10), "Form helper example");
    gui->add_group("Basic types");
    gui->add_variable("Amount of particles:", a, true);

    screen.set_visible(true);
    screen.perform_layout();

    // nanoguiWindow->center();
    glfwSetWindowUserPointer(window, &screen);
    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *window, double x, double y)
                             {
                                 nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));
                                 screen->cursor_pos_callback_event(x, y);
                             });

    glfwSetMouseButtonCallback(window,
                               [](GLFWwindow *window, int button, int action, int modifiers)
                               {
                                   nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                                   screen->mouse_button_callback_event(button, action, modifiers);
                               });

    glfwSetKeyCallback(window,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                           screen->key_callback_event(key, scancode, action, mods);
                       });

    glfwSetCharCallback(window,
                        [](GLFWwindow *window, unsigned int codepoint)
                        {
                            nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                            screen->char_callback_event(codepoint);
                        });

    glfwSetDropCallback(window,
                        [](GLFWwindow *window, int count, const char **filenames)
                        {
                            nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                            screen->drop_callback_event(count, filenames);
                        });

    glfwSetScrollCallback(window,
                          [](GLFWwindow *window, double x, double y)
                          {
                              nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                              screen->scroll_callback_event(x, y);
                          });

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int width, int height)
                                   {
                                       nanogui::Screen *screen = reinterpret_cast<nanogui::Screen *>(glfwGetWindowUserPointer(window));

                                       screen->resize_callback_event(width, height);
                                   });

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
        generator.Bind();
        ps.Draw(camera, density);
        // rec.Draw(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
        //(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        // Draw nanogui
        screen.draw_widgets();
        ps.SetPactilesAmount(a);
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
