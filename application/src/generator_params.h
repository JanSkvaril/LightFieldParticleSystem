#ifndef GENERATOR_CONFIG_CLASS_DEF
#define GENERATOR_CONFIG_CLASS_DEF
#include <glm/vec3.hpp>

// Special parameters for generator, changing these parameters should
// reset cache table - and regenerate light field
struct GeneratorParameters
{
    // model static rotation
    glm::vec3 model_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    // model space position offest
    glm::vec3 model_offset = glm::vec3(0.0f, 0.0f, 0.0f);
    // color used for static light - if enabled
    glm::vec3 model_light_color = glm::vec3(0.0f, 0.7f, 1.0f);
    // should be model rendered with static light?
    bool use_light = true;
};

#endif