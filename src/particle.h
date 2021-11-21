#ifndef PARTICLE_CLASS_DEF
#define PARTICLE_CLASS_DEF
#include <glm/vec3.hpp>

struct Particle
{
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    int time_to_live;
};

#endif