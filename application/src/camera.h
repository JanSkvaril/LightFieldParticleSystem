#ifndef CAMERA_CLASS_DEF
#define CAMERA_CLASS_DEF

#include "glm/glm.hpp"

// scene camera, contains view matrix
class Camera
{
public:
    Camera(glm::ivec2 resolution);
    // camera is placed at position and rotated to look at target
    void LookAt(glm::vec3 position, glm::vec3 target);
    // camera will rotated by given amount around target position
    void RotateAroundTarget(glm::vec2 amount);
    // returns view matrix
    glm::mat4 GetMatrix() const;
    // returns camera position
    glm::vec3 GetPosition() const;
    glm::vec3 GetPosition(glm::vec2 additional_rotation);
    // returns between target and camera
    glm::vec2 GetAngleToTarget() const;
    // returns camera up vector
    glm::vec3 GetUpVector() const;

    // camera resolution
    const glm::ivec2 Resolution;

    // zoom by amount, negative value will zoom out
    void Zoom(float amount);

protected:
    glm::vec2 rotation = glm::vec2(0.0f);
    glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 target;
    glm::vec3 camera_position;
    glm::mat4 view;
};

#endif