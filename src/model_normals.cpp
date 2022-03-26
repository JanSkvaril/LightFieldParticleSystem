#include "model_normals.h"

ModelNormals::ModelNormals(std::string path)
{
    shader = std::make_unique<Shader>("shaders/basic_vertex.vs", "shaders/basic_normal_fragment.fs");
    LoadFromFile(path);
    Setup();
}
