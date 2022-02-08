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
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    void Reset();
    void Update();
    void Draw();

protected:
    std::vector<Particle> particles;
    std::vector<glm::vec3> positions;
    float time = 12145.0f;
    Shader shader;
    void ResetParticle(Particle &particle);
    void DrawParticle(Particle &particle);
    void SortByDepth();
    void CreateVAO();
    void BindPositionVBO();
    unsigned int VAO, VBO;
    unsigned int position_VBO;
    const int amount_of_particles;
};

#endif