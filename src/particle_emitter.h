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
#include "texture.h"
#include "camera.h"
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    void Reset();
    void Update();
    void Draw(Camera &camera, float texture_density);
    void SetPactilesAmount(int amount);
    void SetGravity(float strength, glm::vec3 direction);
    void ShouldShowBorders(bool show_borders);
    void SetSpeed(float speed);
    void SetTimeToLive(int starting, int dispersion);
    void AddTextureHandle(GLuint64 handle);

protected:
    std::vector<GLuint64> texture_handles;
    int show_borders = 0;
    std::vector<Particle> particles;
    std::vector<glm::vec3> positions;
    float time = 12145.0f;
    Shader shader;
    void ResetParticle(Particle &particle);
    void DrawParticle(Particle &particle);
    void SortByDepth(Camera &camera);
    void CreateVAO();
    void BindPositionVBO();
    unsigned int VAO, VBO;
    unsigned int position_VBO;
    const int amount_of_particles;

    float gravity_strength = 0.0f;
    glm::vec3 gravity_dir = glm::vec3(0.0f, 1.0f, 0.0f);
    float speed = 0.1f;
    int starting_ttl = 250;
    int ttl_dispersion = 100;
};

#endif