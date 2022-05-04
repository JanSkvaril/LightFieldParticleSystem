#include "generator.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/epsilon.hpp"
#include <iostream>
Generator::Generator(std::shared_ptr<Model> model, int density, int res) : gt(model, res / density, res / density),
                                                                           t_size(res)
{
    SetParams(std::make_shared<GeneratorParameters>());
    this->density = density;
    this->model = model;
    CreateRenderTexture();
    ResetCache();
}

void Generator::CreateRenderTexture()
{
    // frame buffer
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    // texture
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_size, t_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // filtering: mipmaping!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateTextureMipmap(renderedTexture);

    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t_size, t_size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);
}

void Generator::Generate()
{
    // bind and prep frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, t_size, t_size);

    // calculate rotation offset
    const int range = (density - 1) / 2;
    const float rec_site = 1.0f / (density);
    const float rotation = -glm::two_pi<float>() / (density);

    // how many records were generated
    int generated = 0;

    for (int x = 0; x < density; x++)
    {
        for (int y = 0; y < density; y++)
        {
            if (!cache_table.IsActivated(x, y)) // skip rendering, already cached or not used
                continue;
            // rotation vector
            int real_x = x - range;
            int real_y = y - range;
            auto rot = glm::vec3((real_x + range) * rotation, (real_y + range) * rotation, 0.0f);
            // render to generator texture
            gt.Generate(rot, glm::vec3(0.0f, 0.0f, 0.0f), generator_params);
            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            glViewport(0, 0, t_size, t_size);

            // draw rectangle with texture on light field texture
            gt.BindTexture();
            rectangle.Draw(glm::vec2(real_x * 2.0f, real_y * 2.0f), glm::vec2(rec_site, rec_site));

            // mark as cached in cache table
            cache_table.MarkCached(x, y);
            generated++;
        }
    }
    // if any records were generated, mipmap must be regenerated
    if (generated != 0)
    {
        glGenerateTextureMipmap(renderedTexture);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Generator::GetTexture() const
{
    return renderedTexture;
}

void Generator::Bind()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
}

int Generator::GetDensity() const
{
    return density;
}

void Generator::SetModelRotation(glm::vec3 rotation)
{
    // dont update if rotation has not changed
    auto e = glm::epsilonEqual(rotation, generator_params->model_rotation, 0.01f);
    if (!e.x || !e.y || !e.z)
    {
        generator_params->model_rotation = rotation;
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
    texture_handle = glGetTextureHandleARB(renderedTexture);
    MakeResident();
    return texture_handle;
}

GLuint64 Generator::GetHandle() const
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
    // TODO: remove
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

void Generator::SetParams(std::shared_ptr<GeneratorParameters> params)
{
    this->generator_params = params;
}

void Generator::SetLightColor(glm::vec3 color)
{
    this->generator_params->model_light_color = color;
    ResetCache();
}

void Generator::ShouldUseLight(bool use_light)
{
    this->generator_params->use_light = use_light;
    ResetCache();
}

void Generator::SetModel(std::shared_ptr<Model> model)
{
    this->model = model;
    gt.SetModel(model);
    ResetCache();
}

std::shared_ptr<GeneratorParameters> Generator::GetCurrentParams() const
{
    return generator_params;
}

void Generator::Dispose()
{
    MakeNonResident();
    Delete();
}
