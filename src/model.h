#ifndef MODEL_CLASS_DEF
#define MODEL_CLASS_DEF
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "shader.h"
#include "model_vertex.h"
#include <glm/vec3.hpp>
#include "camera.h"
class Model
{
public:
    Model(std::string path);
    void Draw();
    void Draw(glm::vec3 rotation, glm::vec3 position);
    void Draw(Camera &camera);

protected:
    float time = 0.0f;
    Shader shader;
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indicies;
    std::vector<ModelVertex> verticies;
    void Setup();
};
#endif