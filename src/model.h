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
class Model
{
public:
    struct ModelObjectBuffers
    {
        unsigned int VAO, VBO, EBO;
        std::vector<unsigned int> *indicies;
        std::vector<ModelVertex> *verticies;
    };

    Model(std::string path);
    Model();
    virtual void Draw();
    virtual void Draw(glm::vec3 rotation, glm::vec3 position, std::shared_ptr<GeneratorParameters> generator_params);
    virtual void Draw(Camera &camera);
    ModelObjectBuffers GetObjectBuffers();

protected:
    void LoadFromFile(std::string path);
    float time = 0.0f;
    std::unique_ptr<Shader> shader;
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indicies;
    std::vector<ModelVertex> verticies;
    virtual void Setup();
};
#endif