#ifndef GENERATOR_CLASS_DEF
#define GENERATOR_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "text_rectangle.h"
#include "generator_texture.h"
#include "generator_params.h"
#include "angle_cachetable.h"
class Generator
{
public:
    Generator(Model *model, int density, int res);
    void Generate();
    GLuint GetTexture();
    void Bind();
    int GetDensity();
    void SetModelRotation(glm::vec3 rotation);
    void ChangeDensity(int density);
    void ChangeResolution(int res);
    GLuint64 CreateHandle();
    GLuint64 GetHandle();
    void ResetCache();
    void NotifyChangeAtAngle(glm::ivec2 position);
    void Regenerate();
    AngleCacheTable &GetCacheTable();

protected:
    bool has_handle = false;
    // true when image at xy should rerender
    AngleCacheTable cache_table;
    void MakeResident();
    void ClearTexture();
    void MakeNonResident();
    void Delete();
    GeneratorParameters generator_params;
    GLuint64 texture_handle;
    int density;
    GLsizei t_size;
    void CreateRenderTexture();
    Model *model;
    TextReactangle rectangle;
    GeneratorTexture gt;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif