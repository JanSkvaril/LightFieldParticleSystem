#ifndef MODEL_CLASS_DEF
#define MODEL_CLASS_DEF
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "shader.h"
#include "model_vertex.h"
#include <glm/vec3.hpp>
#include "camera.h"
#include "generator_params.h"
#include <memory>

// basic model for generating light field
// contains 3D object loaded from file
class Model
{
protected:
    // loaded 3D object
    struct ModelObjectBuffers
    {
        // TODO: refactor (smart pointers, or seperate class of 3D object)
        unsigned int VAO, VBO, EBO;
        std::vector<unsigned int> *indicies;
        std::vector<ModelVertex> *verticies;
    };

public:
    Model(std::string path);
    Model();
    // basic draw
    virtual void Draw();
    // draw used by generator
    virtual void Draw(glm::vec3 rotation, glm::vec3 position, std::shared_ptr<GeneratorParameters> generator_params);
    // draw with camera - for drawing in scenes
    virtual void Draw(Camera &camera);

    // returns object containg pointers to 3D object buffers
    ModelObjectBuffers GetObjectBuffers();

protected:
    void LoadFromFile(std::string path);
    float time = 0.0f;
    std::unique_ptr<Shader> shader;
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indicies;
    std::vector<ModelVertex> verticies;

    // loads model using assimp
    virtual void Setup();
};
#endif