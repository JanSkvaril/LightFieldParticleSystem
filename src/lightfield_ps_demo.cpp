#include "lightfield_ps_demo.h"
#include "particle_random_model.h"
#include "particle_leaf.h"
#include "model_normals.h"
#include "particle_starship.h"
#include "particle_halfchange.h"
#include "particle_flower.h"
#include <iostream>
LightFieldPsDemo::LightFieldPsDemo(glm::ivec2 resolution) : particles(100), camera(resolution)
{
    LoadSkyboxes();
}

void LightFieldPsDemo::Update()
{
    UpdatePresetSpecific();
    // ui.HandleCameraControls(camera);
    if (!using_standard_3d)
    {
        particles.Update();
        particles.SortByDepth(camera);
        particles.UpdateBuffers();
        particles.GetRequiredAngles(generator_store, camera, generator_store.GetDensity());
    }
    else
    {
        bench->Update();
    }
}

void LightFieldPsDemo::Draw()
{
    if (!using_standard_3d)
    {
        particles.Draw(camera, generator_store.GetDensity());
    }
    else
    {
        bench->Draw(camera);
    }
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
}

void LightFieldPsDemo::SetPresetBenchmark()
{
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    particles.SetGravity(0.003f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    particles.SimulateSteps(10000);
}

void LightFieldPsDemo::SetPresetNoLightfield(int particles)
{
    ParticleParameters params{};
    params.gravity_strength = 0.003f;
    params.gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    params.amount_of_pacticles = particles;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    bench = std::make_unique<ParticleStandard3d>(std::make_shared<Model>("models/baloon.obj"), params);
    using_standard_3d = true;
    bench->SimulateSteps(10000);
}

void LightFieldPsDemo::SetPresetBenchmarkComplex()
{
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/bunny.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    particles.SetGravity(0.003, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    particles.SimulateSteps(10000);
}

void LightFieldPsDemo::SetPresetNoLightfieldComplex(int particles)
{
    ParticleParameters params{};
    params.gravity_strength = 0.003f;
    params.gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    params.amount_of_pacticles = particles;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    bench = std::make_unique<ParticleStandard3d>(std::make_shared<Model>("models/bunny.obj"), params);
    using_standard_3d = true;
    bench->SimulateSteps(10000);
}

void LightFieldPsDemo::SetPresetStarships()
{
    active_skybox = 1;
    particles.SetParticleProtype(std::make_unique<ParticleStarhip>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    int density = 31;

    loaded_models.push_front(std::make_shared<Model>("models/battleship.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.77, 0.2, 0.27));
    loaded_models.push_front(std::make_shared<Model>("models/battleship2.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.2, 0.27, 0.4));
    loaded_models.push_front(std::make_shared<Model>("models/battleship3.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.3, 0.3, 0.3));
    generator_store.Generators.back()->SetModelRotation(glm::vec3(0.0, 3.14f / 2.0f, 0.0));
    generator_store.SetDensity(density);
    particles.SetGravity(0.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    particles.SetTimeToLive(1000, 800);
    particles.SetPactilesAmount(5000);
    particles.SimulateSteps(10000);
}

void LightFieldPsDemo::LoadSkyboxes()
{
    std::vector<std::string> faces = {
        "imgs/skybox/basic/right.jpg",
        "imgs/skybox/basic/left.jpg",
        "imgs/skybox/basic/top.jpg",
        "imgs/skybox/basic/bottom.jpg",
        "imgs/skybox/basic/front.jpg",
        "imgs/skybox/basic/back.jpg"};
    loaded_skyboxes.push_back(std::make_unique<Skybox>(faces));

    faces = {
        "imgs/skybox/space/skybox_left.png",
        "imgs/skybox/space/skybox_right.png",
        "imgs/skybox/space/skybox_up.png",
        "imgs/skybox/space/skybox_down.png",
        "imgs/skybox/space/skybox_front.png",
        "imgs/skybox/space/skybox_back.png"};

    loaded_skyboxes.push_back(std::make_unique<Skybox>(faces));
    faces = {
        "imgs/skybox/field1/posx.jpg",
        "imgs/skybox/field1/negx.jpg",
        "imgs/skybox/field1/posy.jpg",
        "imgs/skybox/field1/negy.jpg",
        "imgs/skybox/field1/posz.jpg",
        "imgs/skybox/field1/negz.jpg"};
    loaded_skyboxes.push_back(std::make_unique<Skybox>(faces));
    faces = {
        "imgs/skybox/field2/posx.jpg",
        "imgs/skybox/field2/negx.jpg",
        "imgs/skybox/field2/posy.jpg",
        "imgs/skybox/field2/negy.jpg",
        "imgs/skybox/field2/posz.jpg",
        "imgs/skybox/field2/negz.jpg"};
    loaded_skyboxes.push_back(std::make_unique<Skybox>(faces));
}

void LightFieldPsDemo::DrawSkybox()
{
    if (active_skybox < 0)
        return;
    loaded_skyboxes[active_skybox]->Draw(camera);
}

void LightFieldPsDemo::SetPresetHalfChange()
{
    particles.SetParticleProtype(std::make_unique<ParticleHalfChange>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    int density = 21;

    loaded_models.push_front(std::make_shared<Model>("models/cube.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetModelRotation(glm::vec3(3.14 / 4.0f, 3.14 / 4.0f, 1.5));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.7, 0.3, 0.12));
    loaded_models.push_front(std::make_shared<Model>("models/donut.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    generator_store.Generators.back()->SetModelRotation(glm::vec3(0.3, 3.14 / 2.0f, 1.5));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.7, 0.3, 0.12));

    generator_store.SetDensity(51);
    particles.SetSpeed(0.2f);
    particles.AddTextureHandle(generator_store);
    particles.SetTimeToLive(100, 100);
    particles.SetPactilesAmount(2000);
    particles.SetGravity(0.05f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.ShouldParticlesRotate(true);
    particles.Reset();
}

void LightFieldPsDemo::SetPresetSunflower()
{
    particles.SetParticleProtype(std::make_unique<ParticleFlower>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.RotateAroundTarget(glm::vec2(0.5, 0.2));

    loaded_models.push_front(std::make_shared<Model>("models/flower_dead.obj"));

    int density = 31;

    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    loaded_models.push_front(std::make_shared<Model>("models/flower_alive.obj"));
    generator_store.Generators.front()->SetLightColor(glm::vec3(0.463, 0.404, 0.0));
    generator_store.SetDensity(density);
    particles.AddTextureHandle(generator_store);
    sunflower_preset = true;
    particles.SetSpeed(0.0f);
    particles.SetPactilesAmount(10000);
    active_skybox = 3;
}

void LightFieldPsDemo::UpdatePresetSpecific()
{
    if (sunflower_preset)
    {
        if (day_counter == max_day_counter / 2)
        {
            generator_store.Generators.front()->SetModel(loaded_models.front().get());
            generator_store.Generators.front()->SetLightColor(glm::vec3(0.663, 0.804, 0.0));
            active_skybox = 2;
        }
        else if (day_counter >= max_day_counter)
        {
            day_counter = 0;
            generator_store.Generators.front()->SetModel(loaded_models.back().get());
            generator_store.Generators.front()->SetLightColor(glm::vec3(0.463, 0.404, 0.0));
            active_skybox = 3;
        }
        camera.RotateAroundTarget(glm::vec2(0.01f, 0.0));
        day_counter++;
    }

    if (disco_preset)
    {
        camera.RotateAroundTarget(glm::vec2(0.01f, 0.0));
        color_counter++;
        if (color_counter >= max_color_counter)
        {
            color_counter = 0;
            generator_store.Generators.front()->SetLightColor(
                glm::vec3(
                    (rand() % 100) / 100.0f,
                    (rand() % 100) / 100.0f,
                    (rand() % 100) / 100.0f));
        }
    }
}

void LightFieldPsDemo::SetPresetDisco()
{
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front().get(), density, 5000));
    particles.AddTextureHandle(generator_store);
    particles.SetSpeed(0.1f);
    particles.SetGravity(0.01f, glm::vec3(0.1f, -0.9f, 0.0f));
    particles.SetTimeToLive(150, 150);
    particles.SetPactilesAmount(3000);
    disco_preset = true;
    active_skybox = -1;
}
