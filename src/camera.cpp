#include "camera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(glm::ivec2 resolution) : Resolution(resolution)
{
    view = glm::mat4(1.0f);
}

glm::mat4 Camera::GetMatrix() const
{
    return view;
}

glm::vec3 Camera::GetPosition() const
{
    glm::vec3 rotated_position;
    rotated_position = glm::rotate(camera_position, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    rotated_position = glm::rotate(rotated_position, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    return rotated_position;
}

void Camera::LookAt(glm::vec3 position, glm::vec3 target)
{
    this->camera_position = position;
    this->target = target;

    auto rotated_position = GetPosition();
    auto up_vector = GetUpVector();
    view = glm::lookAt(rotated_position, target, up_vector);
}

glm::vec2 Camera::GetAngleToTarget() const
{
    glm::vec2 angle = rotation;
    angle /= glm::two_pi<float>();
    return angle;
}

void Camera::RotateAroundTarget(glm::vec2 amount)
{
    rotation += amount;

    if (rotation.x > glm::two_pi<float>())
    {
        rotation.x = rotation.x - glm::two_pi<float>();
    }
    else if (rotation.x < 0.0f)
    {
        rotation.x = glm::two_pi<float>() + rotation.x;
    }
    if (rotation.y > glm::two_pi<float>())
    {
        rotation.y = rotation.y - glm::two_pi<float>();
    }
    else if (rotation.y < 0.0f)
    {
        rotation.y = glm::two_pi<float>() + rotation.y;
    }

    LookAt(camera_position, target);
}

glm::vec3 Camera::GetUpVector() const
{
    glm::vec3 rotated_up;
    rotated_up = glm::rotate(up_vec, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    rotated_up = glm::rotate(rotated_up, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    return rotated_up;
}

glm::vec3 Camera::GetPosition(glm::vec2 additional_rotation)
{
    glm::vec3 rotated_position;
    rotated_position = glm::rotate(camera_position, rotation.y + additional_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    rotated_position = glm::rotate(rotated_position, rotation.x + additional_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    return rotated_position;
}

void Camera::Zoom(float amount)
{
    camera_position.z += amount;
    LookAt(camera_position, target);
}
