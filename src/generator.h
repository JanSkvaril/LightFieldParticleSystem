#ifndef GENERATOR_CLASS_DEF
#define GENERATOR_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "text_rectangle.h"
#include "generator_texture.h"
class Generator
{
public:
    Generator(Model *model, int density, int res);
    void Generate();
    GLuint GetTexture();
    void Bind();
    int GetDensity();

protected:
    int density;
    const GLsizei t_size;
    void CreateRenderTexture();
    Model *model;
    TextReactangle rectangle;
    GeneratorTexture gt;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif