#ifndef UISETTINGS_CLASS_DEF
#define UISETTINGS_CLASS_DEF
#include <glm/vec3.hpp>
#include "particle_params.h"
#include "generator_params.h"
struct UiSettings : public ParticleParameters, public GeneratorParameters
{
    bool show_border = false;
    bool show_light_field = false;
    int density = 21;
    int resolution = 2000;
};
#endif