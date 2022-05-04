#ifndef GENERATOR_CLASS_DEF
#define GENERATOR_CLASS_DEF
#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "text_rectangle.h"
#include "generator_texture.h"
#include "generator_params.h"
#include "angle_cachetable.h"

// main class for generating light field
// light field is stores in bindless texture
class Generator
{
public:
    Generator(std::shared_ptr<Model> model, int density, int res);
    // generate light field - only for activated uv coordinates
    void Generate();
    // returns texture id, but use texture handle instead
    GLuint GetTexture() const;
    // bind texture - it's better to use handle
    void Bind();
    // returns light field density
    int GetDensity() const;
    // change model that should be used
    void SetModel(std::shared_ptr<Model> model);
    // set model static rotating - this wil regenerate light field!
    void SetModelRotation(glm::vec3 rotation);
    // change lf texture density
    void ChangeDensity(int density);
    // change lf texture resolutin
    void ChangeResolution(int res);
    // generate handle, this will make the texture bindless
    GLuint64 CreateHandle();
    // returns texture handle, vit. bindless textures
    GLuint64 GetHandle() const;
    // resets cache table and light field texture
    void ResetCache();
    // obsolete, dont use
    void NotifyChangeAtAngle(glm::ivec2 position);
    // removes texture resindency, calls generate, and makes it resident again
    void Regenerate();
    // changes model static light color - special parameter, cache table will be reseted
    void SetLightColor(glm::vec3 color);
    // should be static light used?  - special parameter, cache table will be reseted
    void ShouldUseLight(bool use_light);
    // returns reference to cachce table
    AngleCacheTable &GetCacheTable();
    // change generator params (special parameters)
    void SetParams(std::shared_ptr<GeneratorParameters> params);
    // returns actualy used generator parameters
    std::shared_ptr<GeneratorParameters> GetCurrentParams() const;

    // dispose generator, should not be used after dispose is called
    void Dispose();

protected:
    // if handle was generated
    bool has_handle = false;
    GLuint64 texture_handle;
    AngleCacheTable cache_table;
    std::shared_ptr<GeneratorParameters> generator_params;
    int density;
    GLsizei t_size;
    std::shared_ptr<Model> model;
    TextReactangle rectangle;
    GeneratorTexture gt;

    // texture and frame buffer
    GLuint FramebufferName = 0;
    GLuint renderedTexture;
    GLuint depthrenderbuffer;

    // texture methods
    void MakeResident();
    void ClearTexture();
    void MakeNonResident();
    // dispose render texture
    void Delete();
    void CreateRenderTexture();
};
#endif