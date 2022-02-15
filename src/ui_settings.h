#ifndef UISETTINGS_CLASS_DEF
#define UISETTINGS_CLASS_DEF
#include <glm/vec3.hpp>
struct UiSettings
{
    int amount_of_pacticles = 100;
    float gravity_strength = 0.05f;
    // glm::vec3 gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    float gravity_direction = -1.0f;
    // float starting_direction_randomnes = 0.0f;
    // glm::vec3 starting_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    bool show_border = false;
    bool show_light_field = false;
    int density = 21;
    int resolution = 1000;

    glm::vec3 model_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
};
#endif