#ifndef UIMANAGER_CLASS_DEF
#define UIMANAGER_CLASS_DEF
#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>
#include "ui_settings.h"
class UiManager
{
public:
    UiManager(GLFWwindow *window);
    void Draw();

    UiSettings config;

protected:
    void InitEvents();
    nanogui::Screen *screen;
    GLFWwindow *window;
    nanogui::FormHelper *gui;
};

#endif