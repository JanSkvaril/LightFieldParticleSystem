#ifndef MODEL_VERTEX_CLASS_DEF
#define MODEL_VERTEX_CLASS_DEF
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
struct ModelVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

#endif