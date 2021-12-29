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

void Model::Draw(float angle, glm::vec3 axis, glm::vec3 position)
{

    shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, angle, axis);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.5f));
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
    Draw(0.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}
