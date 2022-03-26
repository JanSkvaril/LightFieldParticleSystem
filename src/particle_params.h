#ifndef PARTICLE_CONFIG_CLASS_DEF
#define PARTICLE_CONFIG_CLASS_DEF
#include <glm/vec3.hpp>
struct ParticleParameters
{
    float particle_speed = 0.04f;
    int amount_of_pacticles = 50;
    float gravity_strength = 0.05f;
    // glm::vec3 gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    // float starting_direction_randomnes = 0.0f;
    // glm::vec3 starting_direction = glm::vec3(1.0f, 0.0f, 0.0f);
    int starting_time_to_live = 360;
    int time_to_live_dispersion = 350;

    bool random_x_rotation = false;
    bool random_rotation_speed = false;
    float rotation_multiplier = 0.00f;
    float x_rotation_speed = 0.00f;

    int textures_avaiable = 1;
};

#endif