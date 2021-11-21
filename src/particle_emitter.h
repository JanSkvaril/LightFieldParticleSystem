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
#include <cmath>
#include "shader.h"
#include <algorithm>
class ParticleEmitter
{
public:
    ParticleEmitter(int particles);
    void Reset();
    void Update();
    void Draw();

    std::vector<Particle> particles;

protected:
    float time = 0.0f;
    Shader shader;
    void ResetParticle(Particle &particle);
    void Bind();
    void DrawParticle(Particle &particle);
    void SortByDepth();
    void CreateVAO();
    unsigned int VAO, VBO;
    const int amount_of_particles;
};

#endif