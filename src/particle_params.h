#ifndef PARTICLE_CONFIG_CLASS_DEF
#define PARTICLE_CONFIG_CLASS_DEF
#include <glm/vec3.hpp>
struct ParticleParameters
{
    float particle_speed = 0.1f;
    int amount_of_pacticles = 100;
    float gravity_strength = 0.05f;
    // glm::vec3 gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    // float starting_direction_randomnes = 0.0f;
    // glm::vec3 starting_direction = glm::vec3(1.0f, 0.0f, 0.0f);
    int starting_time_to_live = 110;
    int time_to_live_dispersion = 100;
};

#endif