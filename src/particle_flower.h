#ifndef PARTICLE_FLOWER_CLASS_DEF
#define PARTICLE_FLOWER_CLASS_DEF
#include "particle.h"
class ParticleFlower : public Particle
{
public:
    ParticleFlower();
    ParticleFlower(const Particle &rhs);
    std::unique_ptr<Particle> clone();
    void Reset() override;
    void Update(float time) override;

protected:
};
#endif