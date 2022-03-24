#include "lightfield_ps_demo.h"
#include "particle_random_model.h"
#include "particle_leaf.h"
LightFieldPsDemo::LightFieldPsDemo() : particles(100)
{
}

void LightFieldPsDemo::Update()
{
    // ui.HandleCameraControls(camera);
    particles.Update();
    particles.SortByDepth(camera);
    particles.UpdateBuffers();
    particles.GetRequiredAngles(generator_store, camera, generator_store.GetDensity());
}

void LightFieldPsDemo::Draw()
{
    particles.Draw(camera, generator_store.GetDensity());
}

void LightFieldPsDemo::SetPresetBasic()
{
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(Model("models/bird.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    particles.AddTextureHandle(generator_store);
}

void LightFieldPsDemo::Generate()
{
    generator_store.AllGenerate();
}

void LightFieldPsDemo::SetPresetBalloons()
{
    particles.SetParticleProtype(std::make_unique<ParticleRandomModel>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(Model("models/leaf.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(1.0, 0.0, 0.0));
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.0, 0.5, 0.5));
    particles.AddTextureHandle(generator_store);
}
