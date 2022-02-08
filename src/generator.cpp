#include "generator.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
Generator::Generator(Model *model, int density, int res) : gt(model, res / density, res / density),
                                                           t_size(res)
{
    this->density = density;
    this->model = model;
    CreateRenderTexture();
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDisable(GL_DEPTH_TEST);
    const int range = (density - 1) / 2;
    const float rec_site = 1.0f / (density);
    const float rotation = -glm::two_pi<float>() / (density);
    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            gt.Generate(glm::vec3(x * rotation, y * rotation, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
            glViewport(0, 0, t_size, t_size);
            gt.BindTexture();
            rectangle.Draw(glm::vec2(x * 2.0f, y * 2.0f), glm::vec2(rec_site, rec_site));
        }
    }

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
