#ifndef UISETTINGS_CLASS_DEF
#define UISETTINGS_CLASS_DEF
#include <glm/vec3.hpp>
#include "particle_params.h"
struct UiSettings : public ParticleParameters
{
    bool show_border = false;
    bool show_light_field = false;
    int density = 21;
    int resolution = 2000;

    glm::vec3 model_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
};
#endif