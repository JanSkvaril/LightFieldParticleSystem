#ifndef PARTICLE_HALFCHANGE_CLASS_DEF
#define PARTICLE_HALFCHANGE_CLASS_DEF
#include "particle.h"
class ParticleHalfChange : public Particle
{
public:
    ParticleHalfChange();
    ParticleHalfChange(const Particle &rhs);
    std::unique_ptr<Particle> clone();
    void Reset() override;
    void Update(float time) override;
};
#endif