#ifndef LIGHTFIELD_PS_DEMO_CLASS_DEF
#define LIGHTFIELD_PS_DEMO_CLASS_DEF
#include "generator_store.h"
#include "particle_emitter.h"
#include "particle_standart3d.h"
#include "model.h"
#include <glm/vec2.hpp>
#include <list>
#include "skybox.h"

// class handling demo logic
class LightFieldPsDemo
{
public:
    LightFieldPsDemo(glm::ivec2 resolution);
    void Update();
    void Draw();
    void DrawSkybox();
    void Generate();
    void SetPresetBasic();
    void SetPresetBalloons();
    void SetPresetRealLight();
    void SetPresetBenchmark();
    void SetPresetStarships();
    void SetPresetNoLightfield(int particles);
    void SetPresetBenchmarkComplex();
    void SetPresetNoLightfieldComplex(int particles);
    void SetPresetHalfChange();
    void SetPresetSunflower();
    void SetPresetDisco();
    void SetPresetDiscoBench();
    Camera camera;

    ParticleEmitter particles;
    GeneratorStore generator_store;
    void LoadSkyboxes();
    std::string GetSceneComentary();

    void ShouldCameraRotate(bool rotate);

private:
    void Clean();
    void UpdatePresetSpecific();
    int active_skybox = 0;
    std::vector<std::unique_ptr<Skybox>> loaded_skyboxes;
    std::list<std::shared_ptr<Model>> loaded_models;
    std::string scene_comentary = "- no text -";
    bool using_standard_3d = false;
    std::unique_ptr<ParticleStandard3d> bench;
    TextReactangle texture_rectangle;

    bool sunflower_preset = false;
    int day_counter = 0;
    const int max_day_counter = 200;

    bool disco_preset = false;
    int color_counter = 0;
    const int max_color_counter = 30;
    bool rotate_camera_benchmark = false;
};

#endif