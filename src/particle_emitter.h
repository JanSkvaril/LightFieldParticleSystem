#ifndef PARTICLE_EMITTER_CLASS_DEF
#define PARTICLE_EMITTER_CLASS_DEF
#include <vector>
#include "particle.h"
#include <glm/geometric.hpp>
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    void Reset();
    void Update();
    void Draw();

    std::vector<Particle> particles;

protected:
    void ResetParticle(Particle &particle);
    const int amount_of_particles;
};

#endif