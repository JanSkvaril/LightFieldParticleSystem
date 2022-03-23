#include "particle_emitter.h"
#include "glm/gtx/fast_square_root.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <tgmath.h>
#include "camera.h"
#include "glm/gtx/rotate_vector.hpp"
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
      shader("shaders/particle_vertex.vs", "shaders/particle_fragment.fs"),
      Parameters(particle_parameters)
{
    particle_prototype = std::make_unique<Particle>();
    SetPactilesAmount(particles);
    Reset();
    CreateVAO();
}

void ParticleEmitter::Reset()
{
    for (auto &particle : particles)
    {
        particle->SetParticleParameters(&particle_parameters);
        ResetParticle(*particle);
    }
}

void ParticleEmitter::Update()
{
    int i = 0;
    time += 0.01f;
    for (auto &particle : particles)
    {
        particle->Update(time);
        positions[i] = particle->GetPosition();
        uvs[i] = particle->GetUV();
        i++;
    }
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.Reset();
}
#include <iostream>
void ParticleEmitter::Draw(Camera &camera, float texture_density)
{

    SortByDepth(camera);
    BindPositionVBO();
    BindUVVBO();
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
    glUniformHandleui64vARB(shader.GetUniformLocation("allTheSamplers"), texture_handles.size(), &texture_handles[0]);

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

    glGenBuffers(1, &uv_VBO);
    BindUVVBO();

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
    // particle UV
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glVertexAttribDivisor(3, 1);
    // reset
    glBindVertexArray(0);
}

void ParticleEmitter::DrawParticle(Particle &particle)
{
    const auto p_pos = particle.GetPosition();
    glUniform3f(shader.GetUniformLocation("offset"), p_pos.x, p_pos.y, p_pos.z);
    glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_INT);
}

void ParticleEmitter::SortByDepth(Camera &camera)
{
    return;
    std::sort(positions.begin(), positions.end(), [&camera](glm::vec3 p_a, glm::vec3 p_b)
              { 
                  const auto camera_pos = camera.GetPosition();
                  auto dist_a = glm::fastDistance(camera_pos, p_a);
                  auto dist_b = glm::fastDistance(camera_pos, p_b);
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
    for (size_t i = 0; i < amount; i++)
    {
        this->particles[i] = particle_prototype->clone();
    }

    this->positions.resize(amount);
    this->uvs.resize(amount);
    Reset();
}

void ParticleEmitter::SetGravity(float strength, glm::vec3 direction)
{
    particle_parameters.gravity_strength = strength;
    particle_parameters.gravity_direction = direction;
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
    particle_parameters.particle_speed = speed;
}

void ParticleEmitter::SetTimeToLive(int starting, int dispersion)
{
    particle_parameters.starting_time_to_live = starting;
    particle_parameters.time_to_live_dispersion = dispersion;
}

void ParticleEmitter::AddTextureHandle(GLuint64 handle)
{
    texture_handles.push_back(handle);
}

void ParticleEmitter::GetRequiredAngles(AngleCacheTable &angles, Camera &camera, float texture_density)
{
    CalculateUVs(camera);
    for (auto &uv : uvs)
    {
        angles.Activate(uv.x * texture_density, uv.y * texture_density);
    }
}

void ParticleEmitter::BindUVVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particles.size() * 2, &uvs[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::AddTextureHandle(GeneratorStore &store)
{
    texture_handles.clear();
    for (const auto &generator : store.Generators)
    {
        AddTextureHandle(generator->CreateHandle());
    }
}

void ParticleEmitter::GetRequiredAngles(GeneratorStore &store, Camera &camera, float texture_density)
{
    CalculateUVs(camera);
    for (const auto &generator : store.Generators)
    {
        auto &cache_table = generator->GetCacheTable();
        for (auto &uv : uvs)
        {
            cache_table.Activate(uv.x * texture_density, uv.y * texture_density);
        }
    }
}

void ParticleEmitter::SetParticleProtype(std::unique_ptr<Particle> prototype_ptr)
{
    this->particle_prototype = std::move(prototype_ptr);
}

ParticleEmitter::ParticleEmitter(int particles, std::unique_ptr<Particle> particle_prototype_ptr)
    : ParticleEmitter::ParticleEmitter(particles)
{
    SetParticleProtype(std::move(particle_prototype_ptr));
}

bool ParticleEmitter::ShouldShowBorders()
{
    return show_borders;
}

void ParticleEmitter::ParticleRotationSpeed(float amount)
{
    particle_parameters.rotation_multiplier = amount;
}

bool ParticleEmitter::ShouldParticlesRotate(bool rotate)
{
    particle_parameters.random_x_rotation = rotate;
    return rotate;
}

bool ParticleEmitter::ShouldParticlesRotate()
{
    return particle_parameters.random_x_rotation;
}

void ParticleEmitter::CalculateUVs(Camera &camera)
{
    const float pi = 3.1415926538;
    int i = 0;
    auto camera_position = camera.GetPosition();
    for (auto &particle : particles)
    {
        auto rot = particle->GetRotation();

        auto dir = (camera_position * 2.0f) - particle->GetPosition();
        dir = glm::fastNormalize(dir);

        float u = 0.5f + atan2(dir.z, dir.x) / (2.0f * pi);
        float v = 0.5f + (asin(dir.y) / (2.0f * pi));
        if (isnan(u) || isnan(v))
            continue;
        u += rot.x;
        particle->SetUV({u, v});
        auto corrected = particle->GetUV();
        uvs[i++] = corrected;
    }
}
