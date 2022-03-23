#include "lightfield_ps_demo.h"

LightFieldPsDemo::LightFieldPsDemo() : particles(100)
{
}

void LightFieldPsDemo::Update()
{
    // ui.HandleCameraControls(camera);
    particles.Update();
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
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(Model("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    generator_store.AddGenerator(std::make_shared<Generator>(&loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(1.0, 0.0, 0.0));
    particles.AddTextureHandle(generator_store);
}
