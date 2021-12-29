#ifndef GENERATOR_TEXTURE_CLASS_DEF
#define GENERATOR_TEXTURE_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
class GeneratorTexture
{
public:
    GeneratorTexture(Model *model, GLsizei width, GLsizei height);
    GLuint Generate(glm::vec3 rotation, glm::vec3 position);
    void BindTexture();
    void Delete();

protected:
    void Setup();
    Model *model;
    const GLsizei width;
    const GLsizei height;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif