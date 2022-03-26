#include "particle_simulator.h"
#include <algorithm>
#include <execution>
void ParticleSimulator::Update()
{
    time += 0.01f;
    for (auto &particle : Particles)
    {
        particle->Update(time);
    }
    // // std::for_each(std::execution::par, particles.begin(), particles.end(), [&](auto &particle)
    // //               { particle->Update(time); });
}

void ParticleSimulator::Reset(ParticleParameters *params)
{
    for (auto &particle : Particles)
    {
        particle->SetParticleParameters(params);
        particle->Reset();
    }
}

void ParticleSimulator::Resize(int new_size)
{
    this->Particles.resize(new_size);
    ReCreate();
}

void ParticleSimulator::SetParticlePrototype(std::unique_ptr<Particle> prototype)
{
    this->particle_prototype = std::move(prototype);
    ReCreate();
}

void ParticleSimulator::ReCreate()
{
    for (size_t i = 0; i < this->Particles.size(); i++)
    {
        this->Particles[i] = particle_prototype->clone();
    }
}

ParticleSimulator::ParticleSimulator() // : Particles(Particles)
{
    particle_prototype = std::make_unique<Particle>();
}
