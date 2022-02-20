#ifndef GENERATOR_TEXTURE_CLASS_DEF
#define GENERATOR_TEXTURE_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
class GeneratorTexture
{
public:
    GeneratorTexture(Model *model, GLsizei width, GLsizei height);
    GLuint Generate(glm::vec3 rotation, glm::vec3 position);
    void BindTexture();
    void Delete();
    void Resize(GLsizei width, GLsizei height);

protected:
    GLuint64 texture_handle;
    void Setup();
    Model *model;
    GLsizei width;
    GLsizei height;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif