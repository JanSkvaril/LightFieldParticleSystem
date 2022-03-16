#include "ui_manager.h"
UiManager::UiManager(GLFWwindow *window)
{
    screen = new nanogui::Screen();
    gui = new nanogui::FormHelper(screen);

    this->window = window;
    screen->initialize(window, true);

    nanogui::ref<nanogui::Window> nanoguiWindow = gui->add_window(nanogui::Vector2i(10, 10), "Particle system");
    gui->add_group("Simulation");
    gui->add_variable("Amount of particles", config.amount_of_pacticles);
    gui->add_variable("Gravity strength", config.gravity_strength);
    gui->add_variable("Gravity direction", config.gravity_direction.y);
    gui->add_variable("Particle speed", config.particle_speed);
    gui->add_variable("Starting time to live", config.starting_time_to_live);
    gui->add_variable("Time to live dispersion", config.time_to_live_dispersion);
    gui->add_variable("Random X rotation", config.random_x_rotation);
    gui->add_group("Visual");
    gui->add_variable("Show borders", config.show_border);
    gui->add_variable("Show Lightfield", config.show_light_field);
    gui->add_variable("Lightfield density", config.density);
    gui->add_variable("Resolution", config.resolution);

    nanogui::ref<nanogui::Window> nanoguiWindow2 = gui->add_window(nanogui::Vector2i(250, 10), "Model");
    nanoguiWindow2->set_width(200);
    gui->add_group("Position");
    gui->add_variable("Rotation x", config.model_rotation.x);
    gui->add_variable("Rotation y", config.model_rotation.y);
    gui->add_variable("Rotation z", config.model_rotation.z);
    gui->add_group("Light");

    nanogui::ref<nanogui::Window> nanoguiWindow3 = gui->add_window(nanogui::Vector2i(500, 10), "Scene");
    nanoguiWindow3->set_width(200);
    gui->add_group("Skybox");
    gui->add_variable("Show", config.show_skybox);
    gui->add_group("Scene preset");
    gui->add_group("Light");

    nanogui::ref<nanogui::Window> nanoguiWindow4 = gui->add_window(nanogui::Vector2i(750, 10), "Presets");
    nanoguiWindow4->set_width(200);
    gui->add_group("Avaiable presets");
    gui->add_group("Settings");

    screen->set_visible(true);
    screen->perform_layout();
    InitEvents();
}

void UiManager::InitEvents()
{
    glfwSetWindowUserPointer(window, screen);
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
}

void UiManager::Draw()
{
    screen->draw_widgets();
}
