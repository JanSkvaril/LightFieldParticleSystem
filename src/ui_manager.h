#ifndef UIMANAGER_CLASS_DEF
#define UIMANAGER_CLASS_DEF
#include <nanogui/nanogui.h>
#include "ui_settings.h"
#include "camera.h"
#include "lightfield_ps_demo.h"

class UiManager
{
public:
    UiManager(GLFWwindow *window);
    void Draw();
    void HandleCameraControls(Camera &camera);
    UiSettings config;
    void AddLFPS(LightFieldPsDemo *lfps);
    void RefreshUI();

protected:
    float test = 0.0f;
    void PrepNewScene();
    void DisposeModelWindows();
    void CreateModelWindows();
    void UpdateComentaryText();
    std::string scene_comentary = "Generic scene\n Generic scene Generic scene Generic scene Generic scene Generic scene";
    LightFieldPsDemo *lfps = nullptr;
    int active_counter = 0;
    std::list<nanogui::ref<nanogui::Window>> active_model_windows;
    std::list<nanogui::ref<nanogui::Window>> all_model_windows;
    void InitEvents();
    nanogui::Screen *screen;
    GLFWwindow *window;
    nanogui::FormHelper *gui;
    nanogui::TextArea *text_area;
};

#endif