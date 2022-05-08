#include "particle_halfchange.h"
ParticleHalfChange::ParticleHalfChange(const Particle &rhs)
{
}

ParticleHalfChange::ParticleHalfChange()
{
}

std::unique_ptr<Particle> ParticleHalfChange::clone()
{
    return std::make_unique<ParticleHalfChange>();
}

void ParticleHalfChange::Reset()
{
    Particle::Reset();
    position.y = 20.0f;
    texture_id = 0;
}

void ParticleHalfChange::Update(float time)
{
    Particle::Update(time);
    if (position.y < 0.0f) // half time
    {
        texture_id = 1;
    }
}
