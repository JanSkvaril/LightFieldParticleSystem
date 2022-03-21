#ifndef PARTICLE_CLASS_DEF
#define PARTICLE_CLASS_DEF
#include <glm/vec3.hpp>
#include "glm/vec2.hpp"
#include <glm/geometric.hpp>
#include "particle_params.h"

class Particle
{
public:
    Particle(ParticleParameters *params);
    Particle();
    Particle(const Particle &rhs);
    void Reset();
    void Update(float time);
    glm::vec3 GetPosition();
    glm::vec2 GetRotation();
    void SetParticleParameters(ParticleParameters *params);
    glm::vec2 GetUV();
    void SetUV(glm::vec2 uv);

protected:
    ParticleParameters *params;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec2 rotation;
    glm::vec2 uv;
    float x_rotation_speed = 0.0f;
    float speed;
    int time_to_live;
    float random_time_offset = 0.0f;
    ;
};

#endif