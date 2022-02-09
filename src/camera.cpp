#include "camera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera()
{
    view = glm::mat4(1.0f);
}

glm::mat4 Camera::GetMatrix()
{
    return view;
}

glm::vec3 Camera::GetPosition()
{
    auto rotated_position = glm::rotate(camera_position, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    rotated_position = glm::rotate(rotated_position, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    return rotated_position;
}

void Camera::LookAt(glm::vec3 position, glm::vec3 target)
{
    this->camera_position = position;
    this->target = target;

    auto rotated_position = GetPosition();
    view = glm::lookAt(rotated_position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec2 Camera::GetAngleToTarget()
{
    auto position = GetPosition();
    auto angle = glm::vec2(atan2(position.z, position.x), atan2(position.z, position.y));
    angle /= glm::pi<float>();
    angle += 1.0f;
    return angle;
}

void Camera::RotateAroundTarget(glm::vec2 amount)
{
    rotation += amount;
    LookAt(camera_position, target);
}
