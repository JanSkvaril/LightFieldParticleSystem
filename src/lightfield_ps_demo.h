#ifndef LIGHTFIELD_PS_DEMO_CLASS_DEF
#define LIGHTFIELD_PS_DEMO_CLASS_DEF
#include "generator_store.h"
#include "particle_emitter.h"
#include "model.h"

#include <list>

class LightFieldPsDemo
{
public:
    LightFieldPsDemo();
    void Update();
    void Draw();
    void Generate();
    void SetPresetBasic();

    Camera camera;

private:
    std::list<Model> loaded_models;
    GeneratorStore generator_store;
    ParticleEmitter particles;

    TextReactangle texture_rectangle;
};

#endif