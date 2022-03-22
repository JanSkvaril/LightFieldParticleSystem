#include "ui_manager.h"
UiManager::UiManager(GLFWwindow *window)
{
    screen = new nanogui::Screen();
    gui = new nanogui::FormHelper(screen);

    this->window = window;
    screen->initialize(window, true);

    // nanogui::ref<nanogui::Window> nanoguiWindow2 = gui->add_window(nanogui::Vector2i(250, 10), "Model");
    // nanoguiWindow2->set_width(200);
    // gui->add_group("Position");
    // gui->add_variable("Rotation x", config.model_rotation.x);
    // gui->add_variable("Rotation y", config.model_rotation.y);
    // gui->add_variable("Rotation z", config.model_rotation.z);
    // gui->add_group("Light");

    // nanogui::ref<nanogui::Window> nanoguiWindow3 = gui->add_window(nanogui::Vector2i(500, 10), "Scene");
    // nanoguiWindow3->set_width(200);
    // gui->add_group("Skybox");
    // gui->add_variable("Show", config.show_skybox);
    // gui->add_group("Scene preset");
    // gui->add_group("Light");

    // nanogui::ref<nanogui::Window> nanoguiWindow4 = gui->add_window(nanogui::Vector2i(750, 10), "Presets");
    // nanoguiWindow4->set_width(200);
    // gui->add_group("Avaiable presets");
    // gui->add_group("Settings");
    screen->set_visible(true);
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

void UiManager::HandleCameraControls(Camera &camera)
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
}

void UiManager::AddLFPS(LightFieldPsDemo *lfps)
{
    this->lfps = lfps;
    ParticleEmitter *ps = &lfps->particles;
    GeneratorStore *gs = &lfps->generator_store;

    nanogui::ref<nanogui::Window> nanoguiWindow = gui->add_window(nanogui::Vector2i(10, 10), "Particle system");
    gui->add_group("Simulation");
    gui->add_variable<int>(
        "Amount of particles",
        [ps](int value)
        { ps->SetPactilesAmount(value); },
        [ps]()
        { return ps->Parameters.amount_of_pacticles; });

    gui->add_variable<float>(
        "Gravity strength",
        [ps](float value)
        { ps->SetGravity(value, ps->Parameters.gravity_direction); },
        [ps]()
        { return ps->Parameters.gravity_strength; });

    gui->add_variable<float>(
        "Gravity dicection",
        [ps](float value)
        { ps->SetGravity(ps->Parameters.gravity_strength, {0, value, 0}); },
        [ps]()
        { return ps->Parameters.gravity_direction.y; });
    gui->add_variable<float>(
        "Particle speed",
        [ps](float value)
        { ps->SetSpeed(value); },
        [ps]()
        { return ps->Parameters.particle_speed; });
    gui->add_variable<int>(
        "Particle speed",
        [ps](int value)
        { ps->SetTimeToLive(value, ps->Parameters.time_to_live_dispersion); },
        [ps]()
        { return ps->Parameters.starting_time_to_live; });
    gui->add_variable<int>(
        "Time to live dispersion",
        [ps](int value)
        { ps->SetTimeToLive(ps->Parameters.starting_time_to_live, value); },
        [ps]()
        { return ps->Parameters.time_to_live_dispersion; });
    gui->add_variable<bool>(
        "Random starting rotation",
        [ps](bool value)
        { ps->ShouldParticlesRotate(value); },
        [ps]()
        { return ps->Parameters.random_x_rotation; });
    gui->add_variable<float>(
        "Rotation speed",
        [ps](float value)
        { ps->ParticleRotationSpeed(value); },
        [ps]()
        { return ps->Parameters.rotation_multiplier; });
    gui->add_group("Visual");
    gui->add_variable<bool>(
        "Show borders",
        [ps](bool value)
        { ps->ShouldShowBorders(value); },
        [ps]()
        { return ps->ShouldShowBorders(); });
    gui->add_variable("Show Lightfield", config.show_light_field);
    gui->add_variable<int>(
        "Lightfield density",
        [gs](int value)
        { gs->SetDensity(value); },
        [gs]()
        { return gs->GetDensity(); });
    // gui->add_variable<int>(
    //     "Resolution",
    //     [gs](int value)
    //     { gs->SetResolution(value); },
    //     [gs]()
    //     { return gs->GetRelution(); });
    screen->perform_layout();
}
