#include "particle_leaf.h"

void ParticleLeaf::SimulateMovement(float time)
{
    position += direction * speed;
    float g = params->gravity_strength;
    auto gravity_dir = params->gravity_direction;
    gravity_dir.x += sin(time * 2);
    gravity_dir.z += cos(time * 3);
    direction = (1.0f - g) * direction + g * gravity_dir;
    rotation.x += (x_rotation_speed + params->x_rotation_speed) * params->rotation_multiplier;
}

ParticleLeaf::ParticleLeaf()
{
}

ParticleLeaf::ParticleLeaf(const Particle &rhs)
{
}

std::unique_ptr<Particle> ParticleLeaf::clone()
{
    return std::make_unique<ParticleLeaf>();
}

void ParticleLeaf::Reset()
{
    Particle::Reset();
    texture_id = rand() % params->textures_avaiable;
    position = glm::vec3(-(rand() % 100) / 100.0f, 10.0f, 0.0f);
}
