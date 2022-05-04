#ifndef PATICLE_SIMULATOR_CLASS_DEF
#define PATICLE_SIMULATOR_CLASS_DEF
#include "particle.h"
#include "particle_params.h"
#include <vector>
#include <memory>

// for handling and simulationg particle system
class ParticleSimulator
{
public:
    ParticleSimulator();
    // simulate all particles
    void Update();
    // reset particles with new params
    void Reset(ParticleParameters *params); // todo: use smart pointer
    // change amound of particles - this will reset all particles
    void Resize(int new_size);
    // reinicialize all particles
    void ReCreate();
    // change particle prototype - this will reset all particles
    // ALL particles will be created from this prototype
    void SetParticlePrototype(std::unique_ptr<Particle> prototype);

    // todo: make private
    std::vector<std::unique_ptr<Particle>> Particles;

protected:
    float time = 0.0f;
    std::unique_ptr<Particle> particle_prototype;
};

#endif