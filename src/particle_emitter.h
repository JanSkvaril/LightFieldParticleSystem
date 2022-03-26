#ifndef PARTICLE_EMITTER_CLASS_DEF
#define PARTICLE_EMITTER_CLASS_DEF
#include <vector>
#include "particle.h"
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include "shader.h"
#include <algorithm>

#include "camera.h"
#include "particle_params.h"
#include "angle_cachetable.h"
#include "generator_store.h"
#include "particle_simulator.h"
#include <memory>
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    ParticleEmitter(int particles, std::unique_ptr<Particle> particle_prototype_ptr);
    void Reset();
    void Update();
    void Draw(Camera &camera, float texture_density);
    void SetPactilesAmount(int amount);
    void SetGravity(float strength, glm::vec3 direction);
    void ShouldShowBorders(bool show_borders);
    bool ShouldShowBorders();
    bool ShouldParticlesRotate();
    bool ShouldParticlesRotate(bool rotate);
    void ParticleRotationSpeed(float amount);
    void SetSpeed(float speed);
    void SetTimeToLive(int starting, int dispersion);
    void AddTextureHandle(GLuint64 handle);
    void AddTextureHandle(GeneratorStore &store);
    void GetRequiredAngles(AngleCacheTable &angles, Camera &camera, float texture_density);
    void GetRequiredAngles(GeneratorStore &store, Camera &camera, float texture_density);
    void SetParticleProtype(std::unique_ptr<Particle> prototype_ptr);
    const ParticleParameters &Parameters;
    void SortByDepth(Camera &camera);
    void UpdateBuffers();
    void SimulateSteps(int steps);
    void UseBasicShader();
    void UseRealLightShader();

protected:
    ParticleSimulator simulator;
    enum class ParticleShaderType
    {
        Basic,
        RealLight,
    };
    ParticleShaderType active_shader = ParticleShaderType::Basic;
    Shader &GetActiveShader();
    void CalculateUVs(Camera &camera);
    ParticleParameters particle_parameters;

    std::vector<GLuint64> texture_handles;
    int show_borders = 0;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<int> particle_texture_handle;
    float time = 12145.0f;
    Shader shader;
    Shader real_light_shader;
    void ResetParticle(Particle &particle);
    void DrawParticle(Particle &particle);
    void CreateVAO();
    void BindPositionVBO();
    void BindUVVBO();
    void BindTextureVBO();
    unsigned int VAO, VBO;
    unsigned int position_VBO;
    unsigned int uv_VBO;
    unsigned int texture_VBO;
    int amount_of_particles;
};

#endif