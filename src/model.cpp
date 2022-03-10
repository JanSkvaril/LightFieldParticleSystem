#include "model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Model::Model(std::string path) : shader("shaders/basic_vertex.vs", "shaders/basic_fragment.fs")
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, 0);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        exit(69);
    }
    auto mesh = scene->mMeshes[0];
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indicies.push_back(face.mIndices[j]);
    }
    // exit(0);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        auto vert = mesh->mVertices[i];
        auto normal = mesh->mNormals[i];
        auto tx = mesh->mTextureCoords[0][i];
        verticies.push_back(ModelVertex{
            glm::vec3(vert.x, vert.y, vert.z),
            glm::vec3(normal.x, normal.y, normal.z),
            glm::vec2(tx.x, tx.y)});
    }
    // std::cout << "size " << meshes[0]->mNormals[0].x << "\n";
    Setup();
}

void Model::Draw(glm::vec3 rotation, glm::vec3 position, GeneratorParameters &generator_params)
{

    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::translate(model, position);

    model = glm::rotate(model, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, generator_params.model_rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, generator_params.model_rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.5f));
    projection = glm::perspective(glm::radians(45.0f), (float)10000 / (float)10000, 0.1f, 100.0f);
    auto color = generator_params.model_light_color;
    // set emittor object to uniforms
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(shader.GetUniformLocation("light_color"), 1, glm::value_ptr(color));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_INT);
    glBindVertexArray(0);
}

void Model::Setup()
{

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(ModelVertex), &verticies[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int),
                 &indicies[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)0);
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)(sizeof(glm::vec3)));
    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void *)(sizeof(glm::vec3) + sizeof(glm::vec3)));

    glBindVertexArray(0);
}

void Model::Draw()
{
    GeneratorParameters params{};
    Draw(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), params);
}

void Model::Draw(Camera &camera)
{
    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));

    // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    view = camera.GetMatrix();
    projection = glm::perspective(glm::radians(45.0f), (float)10000 / (float)10000, 0.1f, 100.0f);

    // set emittor object to uniforms
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_INT);
    glBindVertexArray(0);
}
