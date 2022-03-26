#ifndef PATICLE_SIMULATOR_CLASS_DEF
#define PATICLE_SIMULATOR_CLASS_DEF
#include "particle.h"
#include "particle_params.h"
#include <vector>
#include <memory>
class ParticleSimulator
{
public:
    ParticleSimulator();
    void Update();
    void Reset(ParticleParameters *params);
    void Resize(int new_size);
    void ReCreate();
    void SetParticlePrototype(std::unique_ptr<Particle> prototype);
    // const std::vector<std::unique_ptr<Particle>> &Particles;

    std::vector<std::unique_ptr<Particle>> Particles;

protected:
    float time = 0.0f;
    std::unique_ptr<Particle> particle_prototype;
};

#endif