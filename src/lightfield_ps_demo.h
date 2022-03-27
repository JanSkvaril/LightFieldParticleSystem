#ifndef LIGHTFIELD_PS_DEMO_CLASS_DEF
#define LIGHTFIELD_PS_DEMO_CLASS_DEF
#include "generator_store.h"
#include "particle_emitter.h"
#include "particle_standart3d.h"
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
    void SetPresetBalloons();
    void SetPresetRealLight();
    void SetPresetBenchmark();
    void SetPresetNoLightfield(int particles);
    void SetPresetBenchmarkComplex();
    void SetPresetNoLightfieldComplex(int particles);
    Camera camera;

    ParticleEmitter particles;
    GeneratorStore generator_store;

private:
    std::list<std::shared_ptr<Model>> loaded_models;
    bool using_standard_3d = false;
    std::unique_ptr<ParticleStandard3d> bench;
    TextReactangle texture_rectangle;
};

#endif