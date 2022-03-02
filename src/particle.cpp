#include "particle.h"
void Particle::Reset()
{
    direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    // particle.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    direction = glm::normalize(direction);
    speed = params->particle_speed;
    time_to_live = params->starting_time_to_live + rand() % params->time_to_live_dispersion;
    position = glm::vec3(.2f, -0.8f, 0.0f);
}

void Particle::Update()
{
    time_to_live--;
    if (time_to_live <= 0)
    {
        Reset();
    }
    else
    {
        position += direction * speed;
        float g = params->gravity_strength;
        direction = (1.0f - g) * direction + g * params->gravity_direction;
    }
}

glm::vec3 Particle::GetPosition()
{
    return position;
}

void Particle::SetParticleParameters(ParticleParameters *params)
{
    this->params = params;
}

Particle::Particle(ParticleParameters *params)
{
    this->params = params;
}

Particle::Particle()
{
    this->params = nullptr;
}
