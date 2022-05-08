#include "particle_starship.h"
ParticleStarhip::ParticleStarhip(const Particle &rhs)
{
}

ParticleStarhip::ParticleStarhip()
{
}

std::unique_ptr<Particle> ParticleStarhip::clone()
{
    return std::make_unique<ParticleStarhip>();
}

void ParticleStarhip::Reset()
{
    texture_id = rand() % params->textures_avaiable;
    direction = glm::vec3(-1.0, 0.0, 0.0);
    speed = params->particle_speed;
    speed += (rand() % 100) / 1500.0f;
    time_to_live = params->starting_time_to_live + rand() % params->time_to_live_dispersion;
    position = glm::vec3(GetRandomStartingPoint(), GetRandomStartingPoint(), GetRandomStartingPoint());
    position += glm::vec3(30.0f, 0.0, 5.0f);
    if (params->random_x_rotation)
    {
        rotation = glm::vec2((rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
    }
    else
    {
        rotation = {0.0f, 0.0f};
    }
    if (params->random_rotation_speed)
    {
        x_rotation_speed = (rand() % 200) / 100.0f - 1.0f;
    }
    else
    {
        x_rotation_speed = 0.0f;
    }
    random_time_offset = (float)rand();
}

float ParticleStarhip::GetRandomStartingPoint()
{
    auto r = rand() % 2000 - 1000;
    return r / 70.0f;
}
