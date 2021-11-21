#include "particle_emitter.h"
ParticleEmitter::ParticleEmitter(int particles) : amount_of_particles(particles)
{
    this->particles.resize(particles);
    Reset();
}

void ParticleEmitter::Reset()
{
    for (auto &particle : particles)
    {
        ResetParticle(particle);
    }
}

void ParticleEmitter::Update()
{
    for (auto &particle : particles)
    {
        particle.time_to_live--;
        if (particle.time_to_live <= 0)
        {
            ResetParticle(particle);
        }
        else
        {
            particle.position += particle.direction * particle.speed;
        }
    }
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    particle.direction = glm::normalize(particle.direction);
    particle.speed = 0.02f;
    particle.time_to_live = rand() % 500;
    particle.position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void ParticleEmitter::Draw()
{
}
