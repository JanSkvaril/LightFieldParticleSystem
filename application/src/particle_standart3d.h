#ifndef PARTICLE_STANDARD3D_CLASS_DEF
#define PARTICLE_STANDARD3D_CLASS_DEF

#include "model.h"
#include "memory"
#include "particle_params.h"
#include "particle_simulator.h"
#include "camera.h"

// particle system rendering particles with standard 3D techniques
class ParticleStandard3d
{
public:
    ParticleStandard3d(std::shared_ptr<Model> model, ParticleParameters params);
    // update all particles
    void Update();
    // draw particles to current framebuffer
    void Draw(Camera &camera);
    // simulate n steps without drawing
    void SimulateSteps(int steps);

protected:
    void Load();
    std::vector<glm::vec3> positions;
    unsigned int VAO, VBO, EBO;
    unsigned int positions_VBO;
    std::unique_ptr<Shader> shader;
    std::shared_ptr<Model> model;
    ParticleSimulator simulator;
    ParticleParameters params;
};

#endif