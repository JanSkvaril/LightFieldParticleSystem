#include "particle_emitter.h"
#include "glm/gtx/fast_square_root.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <tgmath.h>
#include "camera.h"

float atan22(float y, float x)
{
    bool s = (abs(x) > abs(y));
    if (s == true)
    {
        return glm::pi<float>() / 2.0f - glm::atan(x, y);
    }
    else
    {
        return glm::atan(y, x);
    }
}

ParticleEmitter::ParticleEmitter(int particles)
    : amount_of_particles(particles),
      shader("shaders/particle_vertex.vs", "shaders/particle_fragment.fs")
{
    this->particles.resize(particles);
    this->positions.resize(amount_of_particles);
    Reset();
    for (size_t i = 0; i < 1000; i++)
    {
        // Update();
    }

    CreateVAO();
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
    int i = 0;
    time += 0.01f;
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
            float g = gravity_strength;
            particle.direction = (1.0f - g) * particle.direction + g * gravity_dir;
        }
        positions[i] = particle.position;
        i++;
    }
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    // particle.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    particle.direction = glm::normalize(particle.direction);
    particle.speed = this->speed;
    particle.time_to_live = starting_ttl + rand() % ttl_dispersion;
    particle.position = glm::vec3(.2f, -0.8f, 0.0f);
}
#include <iostream>
void ParticleEmitter::Draw(Camera &camera, float texture_density)
{

    SortByDepth(camera);
    BindPositionVBO();
    // texture.Bind();
    //  load shader program
    shader.Use();
    // emittor object
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = camera.GetMatrix();
    auto angle = camera.GetAngleToTarget();
    // std::cout << glm::to_string(camera.GetPosition()) << "\n"
    //            << glm::to_string(camera.GetAngleToTarget()) << "\n\n";

    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    auto camera_pos = camera.GetPosition();
    auto dir = camera_pos - positions[0];
    dir = normalize(dir);
    // std::cout << "Camera Angle: " << glm::to_string(camera.GetAngleToTarget()) << "\n";
    float a = (glm::atan(dir.z, dir.x));
    float b = (glm::atan(dir.y, dir.x));
    // std::cout << "" << b << " " << a << "  " << glm::to_string(dir) << "\n";
    // std::cout << "Direction:    " << glm::to_string(dir) << "\n\n";
    //
    //  set emittor object to uniforms
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform2fv(shader.GetUniformLocation("camera_angle"), 1, glm::value_ptr(angle));
    glUniform3fv(shader.GetUniformLocation("camera_pos"), 1, glm::value_ptr(camera_pos));
    glUniform1f(shader.GetUniformLocation("u_density"), texture_density);
    glUniform1i(shader.GetUniformLocation("show_border"), show_borders);
    glUniformHandleui64ARB(shader.GetUniformLocation("allTheSamplers"), texture_handle);

    // bind VAO
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, GL_UNSIGNED_INT, particles.size());
    // draw particles
    for (auto &particle : particles)
    {
        // DrawParticle(particle);
    }
}

void ParticleEmitter::CreateVAO()
{
    glGenBuffers(1, &position_VBO);
    BindPositionVBO();

    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    // particle position
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glVertexAttribDivisor(2, 1);
    // reset
    glBindVertexArray(0);
}

void ParticleEmitter::DrawParticle(Particle &particle)
{
    const auto p_pos = particle.position;
    glUniform3f(shader.GetUniformLocation("offset"), p_pos.x, p_pos.y, p_pos.z);
    glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_INT);
}

void ParticleEmitter::SortByDepth(Camera &camera)
{
    return;
    std::sort(positions.begin(), positions.end(), [&camera](glm::vec3 p_a, glm::vec3 p_b)
              { 
                  const auto camera_pos = camera.GetPosition();
                  auto dist_a = glm::distance(camera_pos, p_a);
                  auto dist_b = glm::distance(camera_pos, p_b);
                  return dist_a > dist_b; });
}

void ParticleEmitter::BindPositionVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particles.size() * 3, &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::SetPactilesAmount(int amount)
{
    if (amount == particles.size())
        return;
    this->particles.resize(amount);
    this->positions.resize(amount);
    //  Reset();
}

void ParticleEmitter::SetGravity(float strength, glm::vec3 direction)
{
    this->gravity_strength = strength;
    this->gravity_dir = direction;
}

void ParticleEmitter::ShouldShowBorders(bool show_borders)
{
    if (show_borders)
        this->show_borders = 1;
    else
        this->show_borders = 0;
}

void ParticleEmitter::SetSpeed(float speed)
{
    this->speed = speed;
}

void ParticleEmitter::SetTimeToLive(int starting, int dispersion)
{
    starting_ttl = starting;
    ttl_dispersion = dispersion;
}
