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
    void SetSpeed(float speed);
    void SetTimeToLive(int starting, int dispersion);
    void AddTextureHandle(GLuint64 handle);
    void AddTextureHandle(GeneratorStore &store);
    void GetRequiredAngles(AngleCacheTable &angles, Camera &camera, float texture_density);
    void GetRequiredAngles(GeneratorStore &store, Camera &camera, float texture_density);
    void SetParticleProtype(std::unique_ptr<Particle> prototype_ptr);

protected:
    std::unique_ptr<Particle> particle_prototype;
    std::vector<GLuint64> texture_handles;
    int show_borders = 0;
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    float time = 12145.0f;
    Shader shader;
    void ResetParticle(Particle &particle);
    void DrawParticle(Particle &particle);
    void SortByDepth(Camera &camera);
    void CreateVAO();
    void BindPositionVBO();
    void BindUVVBO();
    unsigned int VAO, VBO;
    unsigned int position_VBO;
    unsigned int uv_VBO;
    int amount_of_particles;

    ParticleParameters particle_parameters;
};

#endif