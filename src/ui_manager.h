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
    void UpdateComentaryText();
    std::string scene_comentary = "Generic scene\n Generic scene Generic scene Generic scene Generic scene Generic scene";
    LightFieldPsDemo *lfps = nullptr;
    void InitEvents();
    nanogui::Screen *screen;
    GLFWwindow *window;
    nanogui::FormHelper *gui;
    nanogui::TextArea *text_area;
};

#endif