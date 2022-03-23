#include "particle_random_model.h"
ParticleRandomModel::ParticleRandomModel()
{
}

std::unique_ptr<Particle> ParticleRandomModel::clone()
{
    return std::make_unique<ParticleRandomModel>();
}

ParticleRandomModel::ParticleRandomModel(const Particle &rhs)
{
}

void ParticleRandomModel::Reset()
{
    Particle::Reset();
    texture_id = rand() % 2;
}
