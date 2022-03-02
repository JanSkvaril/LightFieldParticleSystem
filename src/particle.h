#ifndef PARTICLE_CLASS_DEF
#define PARTICLE_CLASS_DEF
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "particle_params.h"

class Particle
{
public:
    Particle(ParticleParameters *params);
    Particle();
    void Reset();
    void Update();
    glm::vec3 GetPosition();
    void SetParticleParameters(ParticleParameters *params);

protected:
    ParticleParameters *params;
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    int time_to_live;
};

#endif