#ifndef PARTICLE_RANDOM_MODEL_CLASS_DEF
#define PARTICLE_RANDOM_MODEL_CLASS_DEF
#include "particle.h"
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