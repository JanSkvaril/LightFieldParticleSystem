#include "particle_standart3d.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ParticleStandard3d::ParticleStandard3d(std::shared_ptr<Model> model, ParticleParameters params)
{
    this->model = model;
    this->params = params;

    simulator.Resize(params.amount_of_pacticles);
    simulator.Reset(&this->params);
    positions.resize(params.amount_of_pacticles);
    Load();
}

void ParticleStandard3d::Update()
{
    simulator.Update();
    for (size_t i = 0; i < simulator.Particles.size(); i++)
    {
        positions[i] = simulator.Particles[i]->GetPosition();
    }
}
#include <iostream>
void ParticleStandard3d::Draw(Camera &camera)
{
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * simulator.Particles.size() * 3, &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader->Use();

    auto obj_data = model->GetObjectBuffers();

    glm::mat4 model = glm::mat4(1.0f);
    float scale = 0.4f;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    glm::mat4 view = camera.GetMatrix();
    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), (float)camera.Resolution.x / (float)camera.Resolution.y, 0.1f, 100.0f);
    // set emittor object to uniforms
    glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(shader->GetUniformLocation("light_color"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.0f)));
    glUniform1i(shader->GetUniformLocation("use_light"), true);

    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDrawElementsInstanced(GL_TRIANGLES, obj_data.indicies->size(), GL_UNSIGNED_INT, 0, simulator.Particles.size());
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}

void ParticleStandard3d::Load()
{
    shader = std::make_unique<Shader>("shaders/particle3d_vertex.vs", "shaders/basic_fragment.fs");
    auto obj = model->GetObjectBuffers();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenBuffers(1, &positions_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * simulator.Particles.size() * 3, &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, obj.verticies->size() * sizeof(ModelVertex), obj.verticies->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indicies->size() * sizeof(unsigned int),
                 obj.indicies->data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)(sizeof(glm::vec3)));
    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)(sizeof(glm::vec3) + sizeof(glm::vec3)));

    // particle position
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void ParticleStandard3d::SimulateSteps(int steps)
{
    for (size_t i = 0; i < steps; i++)
    {
        Update();
    }
}
