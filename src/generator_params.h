#ifndef GENERATOR_CONFIG_CLASS_DEF
#define GENERATOR_CONFIG_CLASS_DEF
#include <glm/vec3.hpp>
struct GeneratorParameters
{
    glm::vec3 model_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 model_offset = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 model_light_color = glm::vec3(0.0f, 0.7f, 1.0f);
};

#endif