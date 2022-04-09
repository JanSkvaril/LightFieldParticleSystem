#include "generator.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/epsilon.hpp"
#include <iostream>
Generator::Generator(Model *model, int density, int res) : gt(model, res / density, res / density),
                                                           t_size(res),
                                                           Parameters(generator_params)
{
    this->density = density;
    this->model = model;
    CreateRenderTexture();
    ResetCache();
}

void Generator::CreateRenderTexture()
{

    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // The texture we're going to render to

    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_size, t_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t_size, t_size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
}

void Generator::Generate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, t_size, t_size);

    // glDisable(GL_DEPTH_TEST);
    const int range = (density - 1) / 2;
    const float rec_site = 1.0f / (density);
    const float rotation = -glm::two_pi<float>() / (density);
    int generated = 0;

    for (int x = 0; x < density; x++)
    {
        for (int y = 0; y < density; y++)
        {
            if (!cache_table.IsActivated(x, y))
                continue; // skip rendering, already cached or not used
            int real_x = x - range;
            int real_y = y - range;
            auto rot = glm::vec3((real_x + range) * rotation, (real_y + range) * rotation, 0.0f);
            // rot += generator_params.model_rotation;
            gt.Generate(rot, glm::vec3(0.0f, 0.0f, 0.0f), generator_params);
            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            glViewport(0, 0, t_size, t_size);
            gt.BindTexture();
            rectangle.Draw(glm::vec2(real_x * 2.0f, real_y * 2.0f), glm::vec2(rec_site, rec_site));
            cache_table.MarkCached(x, y);
            generated++;
        }
    }
    // if (generated != 0)
    // {
    //     std::cout << "Generating: " << generated << "\n";
    // }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Generator::GetTexture()
{
    return renderedTexture;
}

void Generator::Bind()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
}

int Generator::GetDensity()
{
    return density;
}

void Generator::SetModelRotation(glm::vec3 rotation)
{
    auto e = glm::epsilonEqual(rotation, generator_params.model_rotation, 0.01f);
    if (!e.x || !e.y || !e.z)
    {
        generator_params.model_rotation = rotation;
        ResetCache();
    };
}

void Generator::ChangeDensity(int density)
{
    if (this->density != density)
    {
        this->density = density;
        gt.Resize(t_size / density, t_size / density);
        ResetCache();
    }
}

void Generator::Delete()
{
    glDeleteFramebuffers(1, &FramebufferName);
    glDeleteTextures(1, &renderedTexture);
    glDeleteRenderbuffers(1, &depthrenderbuffer);
}

void Generator::ChangeResolution(int res)
{
    if (this->t_size != res)
    {
        this->t_size = res;
        Delete();
        CreateRenderTexture();
        gt.Resize(t_size / density, t_size / density);
        ResetCache();
        Generate();
    }
}

GLuint64 Generator::CreateHandle()
{
    has_handle = true;
    //   Bind();
    texture_handle = glGetTextureHandleARB(renderedTexture);
    MakeResident();
    return texture_handle;
}

GLuint64 Generator::GetHandle()
{
    return texture_handle;
}

void Generator::MakeResident()
{
    if (!has_handle)
        return;
    glMakeTextureHandleResidentARB(texture_handle);
}

void Generator::MakeNonResident()
{
    if (!has_handle)
        return;
    glMakeTextureHandleNonResidentARB(texture_handle);
}

void Generator::Regenerate()
{
    MakeNonResident();
    Generate();
    MakeResident();
}

void Generator::ResetCache()
{

    cache_table.Resize(density);
    cache_table.ClearCache();
    ClearTexture();
}

void Generator::NotifyChangeAtAngle(glm::ivec2 position)
{
    // cache_table[position.x][position.y] = false;
}

void Generator::ClearTexture()
{
    MakeNonResident();
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    MakeResident();
}

AngleCacheTable &Generator::GetCacheTable()
{
    return cache_table;
}

void Generator::SetParams(GeneratorParameters params)
{
    this->generator_params = params;
}

void Generator::SetLightColor(glm::vec3 color)
{
    this->generator_params.model_light_color = color;
    ResetCache();
}

void Generator::ShouldUseLight(bool use_light)
{
    this->generator_params.use_light = use_light;
    ResetCache();
}

void Generator::SetModel(Model *model)
{
    this->model = model;
    gt.SetModel(model);
    ResetCache();
}
