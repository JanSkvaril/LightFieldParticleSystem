#include "rectangle.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Rectangle::Rectangle() : shader("shaders/basic2D_vertex.vs", "shaders/particle_simple_fragment.fs")
{
    Setup();
}

void Rectangle::Draw(glm::vec2 position, glm::vec2 size)
{
    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_INT);
    glBindVertexArray(0);
}

void Rectangle::Setup()
{
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    // reset
    glBindVertexArray(0);
}
