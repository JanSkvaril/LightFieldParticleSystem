#include "lightfield_ps_demo.h"
#include "particle_random_model.h"
#include "particle_leaf.h"
#include "model_normals.h"
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

    loaded_models.push_front(std::make_shared<Model>("models/bird.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
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

    loaded_models.push_front(std::make_shared<Model>("models/leaf.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(1.0, 0.0, 0.0));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.0, 0.5, 0.5));
    particles.AddTextureHandle(generator_store);
    particles.Reset();
}

void LightFieldPsDemo::SetPresetRealLight()
{
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    int density = 21;
    std::string model_path = "models/baloon.obj";
    loaded_models.push_front(std::make_shared<Model>(model_path));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.front()->ShouldUseLight(false);
    loaded_models.push_front(std::make_shared<ModelNormals>(model_path));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    particles.UseRealLightShader();
    particles.AddTextureHandle(generator_store);
    particles.Reset();
    particles.SimulateSteps(1000);
}
