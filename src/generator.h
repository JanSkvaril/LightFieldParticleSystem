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
    void SetModelRotation(glm::vec3 rotation);

protected:
    glm::vec3 default_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

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