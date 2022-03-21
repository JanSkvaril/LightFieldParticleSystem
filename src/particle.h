#ifndef PARTICLE_CLASS_DEF
#define PARTICLE_CLASS_DEF
#include <glm/vec3.hpp>
#include "glm/vec2.hpp"
#include <glm/geometric.hpp>
#include "particle_params.h"
#include "memory"
class Particle
{
public:
    Particle(ParticleParameters *params);
    Particle();
    Particle(const Particle &rhs);
    virtual void Reset();
    virtual void Update(float time);
    glm::vec3 GetPosition();
    glm::vec2 GetRotation();
    void SetParticleParameters(ParticleParameters *params);
    glm::vec2 GetUV();
    void SetUV(glm::vec2 uv);
    virtual std::unique_ptr<Particle> clone();

protected:
    virtual void SimulateMovement(float time);
    virtual void UpdatePhysics(float time);
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