#ifndef MODEL_CLASS_DEF
#define MODEL_CLASS_DEF
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "shader.h"
class Model
{
public:
    Model(std::string path);
    void Draw();

protected:
    Shader shader;
    unsigned int VAO, VBO, EBO;
    std::vector<aiMesh *> meshes;
    std::vector<unsigned int> indicies;
    void Setup();
};
#endif