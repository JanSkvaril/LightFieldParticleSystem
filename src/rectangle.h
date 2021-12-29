#ifndef RECTANGLE_CLASS_DEF
#define RECTANGLE_CLASS_DEF

#include <glm/vec2.hpp>
#include "shader.h"
class Rectangle
{
public:
    Rectangle();
    void Draw(glm::vec2 position, glm::vec2 size);

protected:
    unsigned int VAO, VBO;
    Shader shader;
    void Setup();
};
#endif