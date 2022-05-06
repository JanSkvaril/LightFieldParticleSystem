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

// object rendering particle systems using light field
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    ParticleEmitter(int particles, std::unique_ptr<Particle> particle_prototype_ptr);
   // ParticleEmitter &ParticleEmitter::operator=(const ParticleEmitter &rhs);

    // reset all particles
    void Reset();
    // simulate particles
    void Update();
    // draw to the current framebuffer
    void Draw(Camera &camera, float texture_density);
    // set amoiunt of particles
    void SetPactilesAmount(int amount);
    // change gravity strenght and direction
    void SetGravity(float strength, glm::vec3 direction);
    // should be borders around particles be shown?
    void ShouldShowBorders(bool show_borders);
    // returns true if borders are currently shown
    bool ShouldShowBorders();
    // returns true if particles are rotation
    bool ShouldParticlesRotate();
    // should particles randomly rotate?
    bool ShouldParticlesRotate(bool rotate);
    // set particles rotation speed
    void ParticleRotationSpeed(float amount);
    // set particles speed
    void SetSpeed(float speed);
    // set particles startint TTL
    // random value between 0 and dispersion is added to TTL
    void SetTimeToLive(int starting, int dispersion);
    // add lf texture handle
    void AddTextureHandle(GLuint64 handle);
    // add texture handle from all generators in given generator store
    void AddTextureHandle(GeneratorStore &store);
    void AddTextureHandle(std::shared_ptr<GeneratorStore> store);
    // adds required uv coordinates to cache table
    void GetRequiredAngles(AngleCacheTable &angles, Camera &camera, float texture_density);
    // adds required uiv coordinates to all generators
    void GetRequiredAngles(GeneratorStore &store, Camera &camera, float texture_density);
    // set particle prototype - all particles will be created from this prototype
    void SetParticleProtype(std::unique_ptr<Particle> prototype_ptr);
    // sort particles by depth
    void SortByDepth(Camera &camera);
    // update all buffers
    void UpdateBuffers();
    // simulate n steps without drawing
    void SimulateSteps(int steps);

    // shaders

    void UseBasicShader();
    void UseRealLightShader();

    const ParticleParameters &Parameters;

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
    ParticleParameters particle_parameters{};

    std::vector<GLuint64> texture_handles;
    int show_borders = 0;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<int> particle_texture_handle;
    float time = 12145.0f; // random starting time
    Shader shader;
    Shader real_light_shader;

    // dont use, obsolete
    void ResetParticle(Particle &particle);
    // dont use, obsolete
    void DrawParticle(Particle &particle);

    // generates VAO
    void CreateVAO();

    // bind VBOs

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