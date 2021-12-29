#ifndef GENERATOR_CLASS_DEF
#define GENERATOR_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rectangle.h"
#include "generator_texture.h"
class Generator
{
public:
    Generator(Model *model);
    void Generate();
    GLuint GetTexture();
    void Bind();

protected:
    void CreateRenderTexture();
    Model *model;
    Rectangle rectangle;
    GeneratorTexture gt;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif