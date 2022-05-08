#ifndef PARTICLE_RANDOM_MODEL_CLASS_DEF
#define PARTICLE_RANDOM_MODEL_CLASS_DEF
#include "particle.h"

// modified particle, chooses random texture handle at reset
class ParticleRandomModel : public Particle
{
public:
    ParticleRandomModel();
    ParticleRandomModel(const Particle &rhs);
    std::unique_ptr<Particle> clone();
    void Reset();

protected:
};
#endif