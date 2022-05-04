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
    if (rotate_camera_benchmark)
    {
        camera.RotateAroundTarget(glm::vec2(0.01f, 0.0));
    }
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
    Clean();
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/bird.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    particles.AddTextureHandle(generator_store);

    scene_comentary = "Basic scene";
}

void LightFieldPsDemo::Generate()
{
    generator_store.AllGenerate();
}

void LightFieldPsDemo::SetPresetBalloons()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<ParticleLeaf>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/leaf.obj"));

    int density = 51;
    int resolution = 10000;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 10000));
    generator_store.Generators.back()->SetLightColor({0.949, 0.4, 0.0});
    generator_store.Generators.back()->SetModelRotation({0.149, 0.6, 0.0});
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 10000));
    generator_store.Generators.back()->SetLightColor({0.239, 0.106, 0.0});
    generator_store.Generators.back()->SetModelRotation({0.239, 0.106, 0.0});
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 10000));
    generator_store.Generators.back()->SetLightColor({0.518, 0.192, 0.0});
    generator_store.Generators.back()->SetModelRotation({0.518, 0.42, 0.0});
    particles.AddTextureHandle(generator_store);
    particles.ShouldParticlesRotate(true);
    particles.ParticleRotationSpeed(0.005f);
    particles.SetGravity(0.002f, glm::vec3(-0.2f, -0.9f, 0.0f));
    particles.SetSpeed(0.03f);
    particles.SetPactilesAmount(1000);
    particles.SetTimeToLive(1000, 1000);
    particles.Reset();
    generator_store.SetDensity(density);
    particles.SimulateSteps(10000);

    active_skybox = 4;
    scene_comentary = "Scene containging 3 seperate generators\neach with different light, rotattion, etc.\n \nParticles are created with leaf prototype";
}

void LightFieldPsDemo::SetPresetRealLight()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.RotateAroundTarget(glm::vec2(0.0, 0.3));
    int density = 21;
    std::string model_path = "models/donut.obj";
    loaded_models.push_front(std::make_shared<Model>(model_path));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.Generators.front()->ShouldUseLight(false);
    loaded_models.push_front(std::make_shared<ModelNormals>(model_path));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.SetDensity(31);
    particles.UseRealLightShader();
    particles.AddTextureHandle(generator_store);
    particles.Reset();

    scene_comentary = "This scene is using special shader for dynamic light.\n2 generators are used:\n  - one for model base\n  - second for normals\nTextures are then combinded in fragment shader.\n Try to notice changing shadow on falling particles as they are falling";
}

void LightFieldPsDemo::SetPresetBenchmark()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    particles.SetGravity(0.003f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    scene_comentary = "This is special scene used for benchmarking.\nDuring benchmark, it's rendered with different amounts\nof particles and different resolutions";
    active_skybox = -1;
}

void LightFieldPsDemo::SetPresetNoLightfield(int particles)
{
    Clean();
    ParticleParameters params{};
    params.gravity_strength = 0.003f;
    params.gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    params.amount_of_pacticles = particles;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    bench = std::make_unique<ParticleStandard3d>(std::make_shared<Model>("models/baloon.obj"), params);
    using_standard_3d = true;
    scene_comentary = "This is special scene, because particles are rendered\nby standard 3D technique - no light field is used.\nParicle system is replaced with Standard3DParticle system";
    active_skybox = -1;
    bench->SimulateSteps(1000);
}

void LightFieldPsDemo::SetPresetBenchmarkComplex()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/bunny.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    particles.SetGravity(0.003, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    active_skybox = -1;
}

void LightFieldPsDemo::SetPresetNoLightfieldComplex(int particles)
{
    Clean();
    ParticleParameters params{};
    params.gravity_strength = 0.003f;
    params.gravity_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    params.amount_of_pacticles = particles;
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    bench = std::make_unique<ParticleStandard3d>(std::make_shared<Model>("models/bunny.obj"), params);
    using_standard_3d = true;
    active_skybox = -1;
    bench->SimulateSteps(1000);
}

void LightFieldPsDemo::SetPresetStarships()
{
    Clean();
    active_skybox = 1;
    particles.SetParticleProtype(std::make_unique<ParticleStarhip>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    int density = 31;

    loaded_models.push_front(std::make_shared<Model>("models/battleship.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.77, 0.2, 0.27));
    loaded_models.push_front(std::make_shared<Model>("models/battleship2.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.2, 0.27, 0.4));
    loaded_models.push_front(std::make_shared<Model>("models/battleship3.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.3, 0.3, 0.3));
    generator_store.Generators.back()->SetModelRotation(glm::vec3(0.0, 3.14f / 2.0f, 0.0));
    generator_store.SetDensity(density);
    particles.SetGravity(0.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    particles.SetTimeToLive(1000, 800);
    particles.SetPactilesAmount(5000);
    particles.SimulateSteps(10000);

    scene_comentary = "Scene containg example of particle system usage\nEvery ship type has it's own generator.\nParticles are spawned in square area.\n \nSpace ship particle prototype is used.";
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

    faces = {
        "imgs/skybox/forest/posx.jpg",
        "imgs/skybox/forest/negx.jpg",
        "imgs/skybox/forest/posy.jpg",
        "imgs/skybox/forest/negy.jpg",
        "imgs/skybox/forest/posz.jpg",
        "imgs/skybox/forest/negz.jpg"};
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
    Clean();
    particles.SetParticleProtype(std::make_unique<ParticleHalfChange>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    int density = 21;

    loaded_models.push_front(std::make_shared<Model>("models/cube.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    generator_store.Generators.back()->SetModelRotation(glm::vec3(3.14 / 4.0f, 3.14 / 4.0f, 1.5));
    generator_store.Generators.back()->SetLightColor(glm::vec3(0.7, 0.3, 0.12));
    loaded_models.push_front(std::make_shared<Model>("models/donut.obj"));
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
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
    scene_comentary = "Scene containg particles, that change used texture\nwhen they pass thought specific point.\nThis scene is using 2 generators.\n \nParticles are created with Transform prototype";
}

void LightFieldPsDemo::SetPresetSunflower()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<ParticleFlower>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    camera.RotateAroundTarget(glm::vec2(0.5, 0.2));

    loaded_models.push_front(std::make_shared<Model>("models/flower_dead.obj"));

    int density = 31;

    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    loaded_models.push_front(std::make_shared<Model>("models/flower_alive.obj"));
    generator_store.Generators.front()->SetLightColor(glm::vec3(0.463, 0.404, 0.0));
    generator_store.SetDensity(density);
    particles.AddTextureHandle(generator_store);
    sunflower_preset = true;
    particles.SetSpeed(0.0f);
    particles.SetPactilesAmount(10000);
    active_skybox = 3;
    scene_comentary = "This scene contains sunflower field\nwith changing day and night cycle.\nSunflowers are facing sun only at daytime.\n \nThis particle system is using only 1 generator.\nLight field is recreated ever day and night\nwith different model.\nParticles are placen in rectangle area.\n \nFlower prototype is used.";
}

void LightFieldPsDemo::UpdatePresetSpecific()
{
    if (sunflower_preset)
    {
        if (day_counter == max_day_counter / 2)
        {
            generator_store.Generators.front()->SetModel(loaded_models.front());
            generator_store.Generators.front()->SetLightColor(glm::vec3(0.663, 0.804, 0.0));
            active_skybox = 2;
        }
        else if (day_counter >= max_day_counter)
        {
            day_counter = 0;
            generator_store.Generators.front()->SetModel(loaded_models.back());
            generator_store.Generators.front()->SetLightColor(glm::vec3(0.463, 0.404, 0.0));
            active_skybox = 3;
        }
        camera.RotateAroundTarget(glm::vec2(0.01f, 0.0));
        day_counter++;
    }

    if (disco_preset)
    {
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
    Clean();
    rotate_camera_benchmark = true;
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    particles.AddTextureHandle(generator_store);
    particles.SetSpeed(0.1f);
    particles.SetGravity(0.01f, glm::vec3(0.1f, -0.9f, 0.0f));
    particles.SetTimeToLive(150, 150);
    particles.SetPactilesAmount(3000);
    disco_preset = true;
    active_skybox = -1;
    scene_comentary = "Scene containging 'disco' balloons,\nthat randomly change color.\n \nThis scene contains only 1 generator.\nLight field texture is recreated every color change.\n \nParticles are created with Disco prototyp.";
}

void LightFieldPsDemo::Clean()
{
    generator_store.Clear(); // dispose GPU allocated stuff
    new (&particles) ParticleEmitter(100);
    new (&camera) Camera(camera.Resolution);
    new (&generator_store) GeneratorStore();
    active_skybox = 0;
    generator_store.Clear();
    loaded_models.clear();
    disco_preset = false;
    sunflower_preset = false;
    using_standard_3d = false;
    rotate_camera_benchmark = false;
}

std::string LightFieldPsDemo::GetSceneComentary()
{
    return scene_comentary;
}

void LightFieldPsDemo::ShouldCameraRotate(bool rotate)
{
    rotate_camera_benchmark = rotate;
}

void LightFieldPsDemo::SetPresetDiscoBench()
{
    Clean();
    particles.SetParticleProtype(std::make_unique<Particle>());
    generator_store.Clear();
    camera.LookAt(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    loaded_models.push_front(std::make_shared<Model>("models/baloon.obj"));

    int density = 21;
    generator_store.AddGenerator(std::make_shared<Generator>(loaded_models.front(), density, 5000));
    particles.SetGravity(0.003f, glm::vec3(0.0f, -1.0f, 0.0f));
    particles.AddTextureHandle(generator_store);
    disco_preset = true;
    active_skybox = -1;
    scene_comentary = "-";
}
