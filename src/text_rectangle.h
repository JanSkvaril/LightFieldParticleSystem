#ifndef RECTANGLE_CLASS_DEF
#define RECTANGLE_CLASS_DEF

#include <glm/vec2.hpp>
#include "shader.h"

// simple class for rendering rectangle to current frame buffer
// used for drawing textures
class TextReactangle
{
public:
    TextReactangle();
    void Draw(glm::vec2 position, glm::vec2 size);

protected:
    unsigned int VAO, VBO;
    Shader shader;
    void Setup();
};
#endif