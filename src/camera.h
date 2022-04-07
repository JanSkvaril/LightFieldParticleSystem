#ifndef CAMERA_CLASS_DEF
#define CAMERA_CLASS_DEF

#include "glm/glm.hpp"
class Camera
{
public:
    Camera(glm::ivec2 resolution);
    void LookAt(glm::vec3 position, glm::vec3 target);
    void RotateAroundTarget(glm::vec2 amount);
    glm::mat4 GetMatrix();
    glm::vec3 GetPosition();
    glm::vec3 GetPosition(glm::vec2 additional_rotation);
    glm::vec2 GetAngleToTarget();
    glm::vec3 GetUpVector();
    const glm::ivec2 Resolution;

protected:
    glm::vec2 rotation = glm::vec2(0.0f);
    glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 target;
    glm::vec3 camera_position;
    glm::mat4 view;
};

#endif