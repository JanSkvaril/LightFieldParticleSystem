#include "particle_flower.h"
ParticleFlower::ParticleFlower(const Particle &rhs)
{
}
ParticleFlower::ParticleFlower()
{
}

std::unique_ptr<Particle> ParticleFlower::clone()
{
    return std::make_unique<ParticleFlower>();
}

void ParticleFlower::Reset()
{
    float x = (std::rand() % 200 - 100) / 3.0f;
    float z = (std::rand() % 200 - 100) / 3.0f;
    position = glm::vec3(x, -1.0f, z);
    rotation.x = (std::rand() % 100) / 100.0f;
}

void ParticleFlower::Update(float time)
{
    // nothing
}
