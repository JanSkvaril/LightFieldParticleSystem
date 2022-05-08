#ifndef PARTICLE_CLASS_DEF
#define PARTICLE_CLASS_DEF
#include <glm/vec3.hpp>
#include "glm/vec2.hpp"
#include <glm/geometric.hpp>
#include "particle_params.h"
#include "memory"

// basic particle class
class Particle
{
public:
    Particle(ParticleParameters *params); // todo: change to smart pointer
    Particle();
    Particle(const Particle &rhs);

    // reset particle
    virtual void Reset();
    // update particle
    virtual void Update(float time);

    // returns position vector
    glm::vec3 GetPosition() const;

    // returns particle rotation vector
    glm::vec2 GetRotation() const;

    // sets particle parameters affecting how is particle simulated, drawn, etc.
    void SetParticleParameters(ParticleParameters *params);

    // returns assigned (u,v) coordinates
    glm::vec2 GetUV() const;
    // sets calculated (u,v) coordinates
    void SetUV(glm::vec2 uv);

    // creates copy of this particle - used for prototype design pattern
    virtual std::unique_ptr<Particle> clone();

    // returns texture handle ID that this particle wants to use
    int GetTextureID();

    // particle current distance
    float distance;

protected:
    // simulate how particle moves in space
    virtual void SimulateMovement(float time);
    // simulate physical movent in space
    virtual void UpdatePhysics(float time);

    int texture_id = 0;
    ParticleParameters *params;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec2 rotation;
    glm::vec2 uv;
    float x_rotation_speed = 0.0f;
    float speed;
    int time_to_live;
    float random_time_offset = 0.0f;
};

#endif