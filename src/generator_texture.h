#ifndef GENERATOR_TEXTURE_CLASS_DEF
#define GENERATOR_TEXTURE_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include "generator_params.h"
#include <memory>
// Contains render texture used for rendering model.
// This texture is rendered to light field texture buffer in generator
class GeneratorTexture
{
public:
    GeneratorTexture(std::shared_ptr<Model> model, GLsizei width, GLsizei height);
    // render model with given parameters to render texture
    GLuint Generate(glm::vec3 rotation, glm::vec3 position, std::shared_ptr<GeneratorParameters> generator_params);
    // bind texture to context
    void BindTexture();
    // dispose all buffers
    void Delete();
    // change used model
    void SetModel(std::shared_ptr<Model> model);
    // resize texture
    void Resize(GLsizei width, GLsizei height);

protected:
    GLuint64 texture_handle;
    void Setup();
    std::shared_ptr<Model> model;
    GLsizei width;
    GLsizei height;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif