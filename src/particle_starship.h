#ifndef PARTICLE_STARSHIP_CLASS_DEF
#define PARTICLE_STARSHIP_CLASS_DEF
#include "particle.h"

// modified particle for space flotile scene
class ParticleStarhip : public Particle
{
public:
    ParticleStarhip();
    ParticleStarhip(const Particle &rhs);
    std::unique_ptr<Particle> clone();
    virtual void Reset() override;

private:
    float GetRandomStartingPoint();
};
#endif