#include "particle.h"
void Particle::Reset()
{
    direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    // particle.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    direction = glm::normalize(direction);
    speed = params->particle_speed;
    time_to_live = params->starting_time_to_live + rand() % params->time_to_live_dispersion;
    position = glm::vec3(.0f, 5, 0.0f);
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

void Particle::Update(float time)
{
    time += random_time_offset;
    time_to_live--;
    if (time_to_live <= 0)
    {
        Reset();
    }
    else
    {
        position += direction * speed;
        float g = params->gravity_strength;
        auto gravity_dir = params->gravity_direction;
        gravity_dir.x = sin(time * 0.1) * 0.5;
        gravity_dir.z = sin((time + 41) * 0.15) * 0.2;
        direction = (1.0f - g) * direction + g * gravity_dir;
        rotation.x += (x_rotation_speed + params->x_rotation_speed) * params->rotation_multiplier;
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

glm::vec2 Particle::GetRotation()
{
    return rotation;
}

glm::vec2 Particle::GetUV()
{
    return uv;
}

void Particle::SetUV(glm::vec2 uv)
{
    // above 1
    if (uv.x >= 1.0f)
    {
        uv.x -= 1.0f;
    }
    if (uv.y >= 1.0f)
    {
        uv.y -= 1.0f;
    }
    // under 0
    if (uv.x < 0.0f)
    {
        uv.x = 1.0f + uv.x;
    }
    if (uv.y < 0.0f)
    {
        uv.y = 1.0f + uv.y;
    }

    this->uv = uv;
}
