#include "particle_emitter.h"
#include "glm/gtx/fast_square_root.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <tgmath.h>
#include "camera.h"
#include "glm/gtx/rotate_vector.hpp"
//#include <execution>
#include <algorithm>
#include <omp.h>

ParticleEmitter::ParticleEmitter(int particles)
    : amount_of_particles(particles),
      shader("shaders/particle_vertex.vs", "shaders/particle_fragment.fs"),
      real_light_shader("shaders/particle_vertex.vs", "shaders/particle_fragment_light.fs"),
      Parameters(particle_parameters)
{
    SetPactilesAmount(particles);
    Reset();
    CreateVAO();
}

void ParticleEmitter::Reset()
{
    simulator.Reset(&particle_parameters);
}

void ParticleEmitter::Update()
{
    simulator.Update();
}

void ParticleEmitter::ResetParticle(Particle &particle)
{
    particle.Reset();
}
void ParticleEmitter::Draw(Camera &camera, float texture_density)
{
    // bind vbos
    BindPositionVBO();
    BindUVVBO();
    BindTextureVBO();

    //  load shader
    Shader &used_shader = GetActiveShader();
    used_shader.Use();

    // MVP
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // view from camera object
    view = camera.GetMatrix();
    auto angle = camera.GetAngleToTarget();

    // create billboard rotation matrix M
    glm::vec3 camera_normal = glm::vec3(view[0][2], view[1][2], view[2][2]);
    camera_normal = -camera_normal;
    glm::vec3 up = glm::vec3(view[0][1], view[1][1], view[2][1]);
    glm::vec3 r = cross(camera_normal, up);
    r = normalize(r);
    glm::vec3 new_up = cross(camera_normal, r);
    glm::mat3 rot = glm::mat3(r, new_up, camera_normal);

    // create projection matrix
    projection = glm::perspective(glm::radians(45.0f), (float)camera.Resolution.x / (float)camera.Resolution.y, 0.1f, 100.0f);

    // calculate view and mocdel transformation matrix
    // perspective matrix is calculated in the shader
    glm::mat4 viewmodel = view * model;
    glUniformMatrix4fv(used_shader.GetUniformLocation("viewmodel"), 1, GL_FALSE, glm::value_ptr(viewmodel));
    glUniformMatrix4fv(used_shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix3fv(used_shader.GetUniformLocation("rot"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniform1f(used_shader.GetUniformLocation("u_density"), texture_density);
    glUniform1i(used_shader.GetUniformLocation("show_border"), show_borders);
    glUniformHandleui64vARB(used_shader.GetUniformLocation("texture_handles"), texture_handles.size(), &texture_handles[0]);

    // bind VAO
    glBindVertexArray(VAO);

    // draw all particle instances
    glDrawArraysInstanced(GL_TRIANGLES, 0, GL_UNSIGNED_INT, simulator.Particles.size());
}

void ParticleEmitter::CreateVAO()
{
    // gen VBOs
    glGenBuffers(1, &position_VBO);
    BindPositionVBO();

    glGenBuffers(1, &uv_VBO);
    BindUVVBO();

    glGenBuffers(1, &texture_VBO);
    BindTextureVBO();

    // verticies and tex coords for billboard
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // gen VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // prep VBOs
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
    // particle texture handle
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(int) * 1, (void *)0);
    glVertexAttribDivisor(4, 1);
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
    const auto camera_pos = camera.GetPosition();
    // std::for_each(std::execution::par, simulator.Particles.begin(), simulator.Particles.end(), [&camera_pos](auto &p_a)
    //               { p_a->distance = glm::fastDistance(camera_pos, p_a->GetPosition()); });
    std::sort(simulator.Particles.begin(), simulator.Particles.end(), [](auto &p_a, auto &p_b)
              {
                  const auto dist_a = p_a->distance;
                  const auto dist_b = p_b->distance;
                  return dist_a > dist_b; });
}

void ParticleEmitter::BindPositionVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * simulator.Particles.size() * 3, &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::SetPactilesAmount(int amount)
{
    particle_parameters.amount_of_pacticles = amount;
    simulator.Resize(amount);
    simulator.Reset(&particle_parameters);
    this->positions.resize(amount);
    this->uvs.resize(amount);
    this->particle_texture_handle.resize(amount);
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
    particle_parameters.textures_avaiable = texture_handles.size();
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * simulator.Particles.size() * 2, &uvs[0], GL_STATIC_DRAW);
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
    simulator.SetParticlePrototype(std::move(prototype_ptr));
    simulator.Reset(&particle_parameters);
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

    const float pi = 3.1415926538f;
    int i = 0;
    auto camera_position = camera.GetPosition();

#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < simulator.Particles.size(); i++)
        {
            const auto &particle = simulator.Particles[i];
            const auto particle_pos = particle->GetPosition();
            const auto rot = particle->GetRotation();

            // distance - for depth sorting
            particle->distance = glm::fastDistance(camera_position, particle_pos);

            // direction from the camera
            auto dir = (camera_position * 4.0f) - particle_pos;
            dir = glm::fastNormalize(dir);

            // uv mapping, with attempt of optimilazations :)
            float inverse_sqrt = glm::fastInverseSqrt(pow(dir.x, 2.0f) + pow(dir.z, 2.0f));
            float u = acos(dir.x * inverse_sqrt) / (2.0f * pi);
            if (dir.z < 0.0)
            {
                u = 1.0f - u;
            }
            float v = 0.5f + (asin(dir.y) / (2.0f * pi));

            u += rot.x;
            particle->SetUV({u, v});
        }
        // pass calculated uvs to uvs array (used for VBO)
#pragma omp for
        for (int i = 0; i < uvs.size(); i++)
        {
            uvs[i] = simulator.Particles[i]->GetUV();
        }
    }
}

void ParticleEmitter::BindTextureVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * particle_texture_handle.size() * 1, &particle_texture_handle[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEmitter::UpdateBuffers()
{
    int i = 0;
    time += 0.01f;
    for (auto &particle : simulator.Particles)
    {
        positions[i] = particle->GetPosition();
        uvs[i] = particle->GetUV();
        particle_texture_handle[i] = particle->GetTextureID();
        i++;
    }
}

void ParticleEmitter::SimulateSteps(int steps)
{
    for (size_t i = 0; i < steps; i++)
    {
        Update();
    }
}

void ParticleEmitter::UseBasicShader()
{
    active_shader = ParticleShaderType::Basic;
}

void ParticleEmitter::UseRealLightShader()
{
    active_shader = ParticleShaderType::RealLight;
}

Shader &ParticleEmitter::GetActiveShader()
{
    switch (active_shader)
    {
    case ParticleShaderType::RealLight:
        return real_light_shader;
        break;

    default:
        return shader;
        break;
    }
}

void ParticleEmitter::AddTextureHandle(std::shared_ptr<GeneratorStore> store)
{
    texture_handles.clear();
    for (const auto &generator : store->Generators)
    {
        AddTextureHandle(generator->CreateHandle());
    }
}

