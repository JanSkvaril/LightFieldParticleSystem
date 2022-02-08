#include "particle_emitter.h"
#include "glm/gtx/fast_square_root.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
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
        // particle.time_to_live--;
        if (particle.time_to_live <= 0)
        {
            ResetParticle(particle);
        }
        else
        {
            particle.position += particle.direction * particle.speed;
            float g = 0.05f;
            // particle.direction = (1.0f - g) * particle.direction + g * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        positions[i] = particle.position;
        i++;
    }

    SortByDepth();
    BindPositionVBO();
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    // particle.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    particle.direction = glm::normalize(particle.direction);
    particle.speed = 0.000f;
    particle.time_to_live = 200 + rand() % 250;
    particle.position = glm::vec3(0.0f, 0.0f, 0.0f);
}
#include <iostream>
void ParticleEmitter::Draw()
{
    // texture.Bind();
    //  load shader program
    shader.Use();

    // emittor object
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    //  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.8f, 0.0f));
    // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    // view = glm::translate(view, glm::vec3(cos(time * 0.5f) * 0.2, sin(time * 0.5f) * 0.2, -2.5f));
    glm::vec3 camera_pos = glm::vec3(sin(time) * 4.0f, cos(time) * 4.0f, 5.0f);
    view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // camera_pos = glm::normalize(camera_pos);
    camera_pos = glm::vec3(sin(time), cos(time), 0.0f);
    auto test = glm::vec2((camera_pos.x / (1.0f - camera_pos.z)), camera_pos.y / (1.0f - camera_pos.z));
    std::cout << glm::to_string((test + 1.0f) * 0.5f) << "\n\n";

    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    // set emittor object to uniforms
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(shader.GetUniformLocation("camera_pos"), 1, glm::value_ptr(camera_pos));

    // bind VAO
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, GL_UNSIGNED_INT, amount_of_particles);
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

void ParticleEmitter::SortByDepth()
{
    std::sort(positions.begin(), positions.end(), [](glm::vec3 p_a, glm::vec3 p_b)
              { 
                    glm::vec3 camera = glm::vec3(0.0f, 0.0f, -5.5f);
                    return p_a.z > p_b.z; });
}

void ParticleEmitter::BindPositionVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * amount_of_particles * 3, &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
