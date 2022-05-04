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
    Generator(std::shared_ptr<Model> model, int density, int res);
    void Generate();
    GLuint GetTexture();
    void Bind();
    int GetDensity();
    void SetModel(std::shared_ptr<Model> model);
    void SetModelRotation(glm::vec3 rotation);
    void ChangeDensity(int density);
    void ChangeResolution(int res);
    GLuint64 CreateHandle();
    GLuint64 GetHandle();
    void ResetCache();
    void NotifyChangeAtAngle(glm::ivec2 position);
    void Regenerate();
    void SetLightColor(glm::vec3 color);
    void ShouldUseLight(bool use_light);
    AngleCacheTable &GetCacheTable();
    void SetParams(std::shared_ptr<GeneratorParameters> params);
    std::shared_ptr<GeneratorParameters> GetCurrentParams() const;

protected:
    bool has_handle = false;
    // true when image at xy should rerender
    AngleCacheTable cache_table;
    void MakeResident();
    void ClearTexture();
    void MakeNonResident();
    void Delete();
    std::shared_ptr<GeneratorParameters> generator_params;
    GLuint64 texture_handle;
    int density;
    GLsizei t_size;
    void CreateRenderTexture();
    std::shared_ptr<Model> model;
    TextReactangle rectangle;
    GeneratorTexture gt;
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;
};
#endif