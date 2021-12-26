#include "particle_emitter.h"
ParticleEmitter::ParticleEmitter(int particles)
    : amount_of_particles(particles),
      shader("shaders/particle_vertex.vs", "shaders/basic_fragment.fs"),
      texture("imgs/dice.png")
{
    this->particles.resize(particles);
    Reset();
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
    time += 0.1f;
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
    SortByDepth();
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.direction = glm::vec3((rand() % 100) - 50, (rand() % 100 - 50), (rand() % 100) - 50);
    particle.direction = glm::normalize(particle.direction);
    particle.speed = 0.00f;
    particle.time_to_live = rand() % 1000;
    particle.position = glm::vec3(0.0f, 0.0f, 0.0f);
}
#include <iostream>
void ParticleEmitter::Draw()
{
    texture.Bind();
    // load shader program
    shader.Use();

    // emittor object
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    //  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.8f, 0.0f));
    // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    view = glm::translate(view, glm::vec3(cos(time * 0.5f) * 0.2, sin(time * 0.5f) * 0.2, -2.5f));
    // view = glm::rotate(view, glm::radians(-time * 4.0f), glm::vec3(1.0f, 0.8f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    // set emittor object to uniforms
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // bind VAO
    glBindVertexArray(VAO);

    // draw particles
    for (auto &particle : particles)
    {
        DrawParticle(particle);
    }
}

void ParticleEmitter::CreateVAO()
{
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
    return;
    std::sort(particles.begin(), particles.end(), [](Particle &p_a, Particle &p_b)
              { return glm::length(p_a.position) > glm::length(p_b.position); });
}