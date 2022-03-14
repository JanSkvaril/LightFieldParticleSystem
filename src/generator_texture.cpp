#include "generator_texture.h"

GeneratorTexture::GeneratorTexture(Model *model, GLsizei width, GLsizei height) : width(width), height(height)
{
    this->model = model;
    Setup();
}

GLuint GeneratorTexture::Generate(glm::vec3 rotation, glm::vec3 position, GeneratorParameters &generator_params)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glViewport(0, 0, width, height);
    //  glEnable(GL_DEPTH_TEST);
    //  glViewport(0, 0, 1024, 768);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    model->Draw(rotation, position, generator_params);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    return renderedTexture;
}

void GeneratorTexture::BindTexture()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
}

void GeneratorTexture::Setup()
{
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // The texture we're going to render to

    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
}

void GeneratorTexture::Delete()
{
    glDeleteFramebuffers(1, &FramebufferName);
    glDeleteTextures(1, &renderedTexture);
    glDeleteRenderbuffers(1, &depthrenderbuffer);
}

void GeneratorTexture::Resize(GLsizei width, GLsizei height)
{
    Delete();
    this->width = width;
    this->height = height;
    Setup();
}
