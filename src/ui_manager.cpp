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

    nanogui::ref<nanogui::Window> nanoguiWindow3 = gui->add_window(nanogui::Vector2i(500, 10), "Scene");
    nanoguiWindow3->set_width(200);
    gui->add_group("Skybox");
    gui->add_variable("Show", config.show_skybox);
    gui->add_group("Scene preset");
    gui->add_group("Light");

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
    if (hide_ui)
        return;
    screen->draw_widgets();
}

void UiManager::HandleCameraControls(Camera &camera)
{
    const float speed = 0.05f;
    const float zoom_speed = 0.1f;
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.Zoom(zoom_speed);
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.Zoom(-zoom_speed);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        hide_ui = true;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        hide_ui = false;
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
    gui->add_button("Simulate 1000 steps", [ps]()
                    { ps->SimulateSteps(1000); });
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
    gui->add_variable<int>(
        "Resolution",
        [gs, ps](int value)
        {
            gs->SetResolution(value);
            ps->AddTextureHandle(gs);
        },
        [gs]()
        { return gs->GetRelution(); });

    CreateModelWindows();

    auto nanoguiWindow4 = gui->add_window(nanogui::Vector2i(750, 10), "Presets");
    nanoguiWindow4->set_width(200);
    gui->add_group("Avaiable presets");
    gui->add_button("Basic", [&, lfps]()
                    {DisposeModelWindows() ;lfps->SetPresetBasic(); this->PrepNewScene(); });
    gui->add_button("Leafes", [&, lfps]()
                    {DisposeModelWindows(); lfps->SetPresetBalloons();this->PrepNewScene(); });
    gui->add_button("Starships", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetStarships();this->PrepNewScene(); });
    gui->add_button("Transformation", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetHalfChange();this->PrepNewScene(); });
    gui->add_button("Sunflowers", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetSunflower(); this->PrepNewScene(); });
    gui->add_button("Disco", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetDisco(); this->PrepNewScene(); });
    gui->add_button("Dynamic light", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetRealLight(); this->PrepNewScene(); });
    gui->add_button("No light field", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetNoLightfield(1000); this->PrepNewScene(); });
    gui->add_button("Benchark", [&, lfps]()
                    {DisposeModelWindows() ; lfps->SetPresetBenchmark(); this->PrepNewScene(); });
    auto group = gui->add_group("Commentary");

    text_area = new nanogui::TextArea(nanoguiWindow4);
    text_area->append_line("No text");
    gui->add_widget("", text_area);
    screen->set_visible(true);
    screen->perform_layout();
}

void UiManager::RefreshUI()
{
    screen->perform_layout();
}

void UiManager::UpdateComentaryText()
{
    text_area->clear();
    auto text = lfps->GetSceneComentary();
    std::istringstream iss(text);
    for (std::string line; std::getline(iss, line);)
    {
        text_area->append_line(line);
    }
}
#include <iostream>
void UiManager::CreateModelWindows()
{
    GeneratorStore *gs = &lfps->generator_store;
    active_counter++;
    // model
    for (int i = 0; i < gs->Generators.size(); i++)
    {
        nanogui::ref<nanogui::Window> nanoguiWindow2 = gui->add_window(nanogui::Vector2i(250, 10 * (i + 1)), "Model");
        nanoguiWindow2->set_width(200);
        int ac = active_counter;
        gui->add_group("Position");
        gui->add_variable<float>(
            "Rotation x",
            [&, i, ac](float value)
            {
                if (ac != this->active_counter)
                    return;
                auto r = lfps->generator_store.Generators[i]->Parameters.model_rotation;
                lfps->generator_store.Generators[i]->SetModelRotation({value, r.y, r.z});
            },
            [&, i, ac]()
            {
                if (ac != this->active_counter) return 0.0f;
                return lfps->generator_store.Generators[i]->Parameters.model_rotation.x; });
        gui->add_variable<float>(
            "Rotation y",
            [&, i, ac](float value)
            {
                if (ac != this->active_counter)
                    return;
                auto r = lfps->generator_store.Generators[i]->Parameters.model_rotation;
                lfps->generator_store.Generators[i]->SetModelRotation({r.x, value, r.y});
            },
            [&, i, ac]()
            {
                if (ac != this->active_counter) return 0.0f;
                return lfps->generator_store.Generators[i]->Parameters.model_rotation.y; });
        gui->add_variable<float>(
            "Rotation z",
            [&, i, ac](float value)
            {
                if (ac != this->active_counter)
                    return;
                auto r = lfps->generator_store.Generators[i]->Parameters.model_rotation;
                lfps->generator_store.Generators[i]->SetModelRotation({r.x, r.y, value});
            },
            [&, i, ac]()
            {
                if (ac != this->active_counter) return 0.0f;
                return lfps->generator_store.Generators[i]->Parameters.model_rotation.z; });
        gui->add_group("Light");
        gui->add_variable<bool>(
            "Use baked-in light",
            [&, i, ac](bool value)
            {
                if (ac != this->active_counter)
                    return;
                lfps->generator_store.Generators[i]->ShouldUseLight(value);
            },
            [&, i, ac]()
            {
                if (ac != this->active_counter)
                    return false;
                return lfps->generator_store.Generators[i]->Parameters.use_light;
            });
        gui->add_variable<nanogui::Color>(
            "Color",
            [&, i, ac](nanogui::Color value)
            {
                if (ac != this->active_counter)
                    return;
                glm::vec3 col = glm::vec3(value.r(), value.g(), value.b());
                lfps->generator_store.Generators[i]->SetLightColor(col);
            },
            [&, i, ac]()
            {
                if (ac != this->active_counter)
                    return nanogui::Color();
                glm::vec3 col = lfps->generator_store.Generators[i]->Parameters.model_light_color;
                return nanogui::Color{col.r, col.g, col.b, 1.0f};
            });
        active_model_windows.push_back(nanoguiWindow2);
        all_model_windows.push_back(nanoguiWindow2);
    }
}

void UiManager::PrepNewScene()
{
    UpdateComentaryText();
    CreateModelWindows();
    gui->refresh();
    screen->perform_layout();
}

void UiManager::DisposeModelWindows()
{

    for (auto &model_window : active_model_windows)
    {
        model_window->set_visible(false);
        model_window->set_enabled(false);
        model_window->dispose();
    }
    gui->refresh();
    screen->nvg_flush();
    screen->perform_layout();
    active_model_windows.clear();
}
