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

protected:
    LightFieldPsDemo *lfps = nullptr;
    void InitEvents();
    nanogui::Screen *screen;
    GLFWwindow *window;
    nanogui::FormHelper *gui;
};

#endif