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

glm::vec2 Camera::GetAngleToTarget()
{
    auto position = GetPosition();
    position = glm::normalize(position);

    // auto angle = glm::vec2(atan2(position.x, position.z), acosf(position.y / 1.0f));
    // angle.x /= glm::two_pi<float>();
    // angle.y /= glm::pi<float>();
    // if (angle.x < 0.0)
    // {
    //     angle.x = 1.0f + angle.x;
    // }

    // if (angle.y < 0.5)
    // {
    //     // angle.y = 1.0f - angle.y;
    // }
    glm::vec2 angle = rotation;
    return angle;
}

void Camera::RotateAroundTarget(glm::vec2 amount)
{
    rotation += amount;

    if (rotation.x > 1.0f)
    {
        rotation.x = rotation.x - 1.0f;
    }
    else if (rotation.x < 0.0f)
    {
        rotation.x = 1.0f + rotation.x;
    }
    if (rotation.y > 1.0f)
    {
        rotation.y = rotation.y - 1.0f;
    }
    else if (rotation.y < 0.0f)
    {
        rotation.y = 1.0f + rotation.y;
    }

    LookAt(camera_position, target);
}

glm::vec3 Camera::GetUpVector()
{
    glm::vec3 rotated_up;
    rotated_up = glm::rotate(up_vec, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    rotated_up = glm::rotate(rotated_up, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    return rotated_up;
}
