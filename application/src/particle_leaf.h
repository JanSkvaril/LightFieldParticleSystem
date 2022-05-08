#ifndef PARTICLE_LEAF_CLASS_DEF
#define PARTICLE_LEAF_CLASS_DEF
#include "particle.h"

// modified particle for leafes scene
class ParticleLeaf : public Particle
{
public:
    ParticleLeaf();
    ParticleLeaf(const Particle &rhs);
    std::unique_ptr<Particle> clone();
    void Reset() override;

protected:
    virtual void SimulateMovement(float time) override;
};
#endif