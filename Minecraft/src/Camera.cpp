#include "Camera.hpp"
#include "Input.hpp"
#include <iostream>

Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position), m_up(up), m_worldUp(up), m_pitch(pitch), m_yaw(yaw), m_speed(SPEED), m_sensitivity(SENSITIVITY) {
    updateVectors();
}

void Camera3D::update(float dt) {
    if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        m_speed = 40;
    else
        m_speed = 10;
    static float lastX = 0, lastY = 0;
    static bool first = true;

    float velotcity = m_speed * dt;
    if (Input::isKeyPressed(GLFW_KEY_W))
        m_position += m_front * velotcity;
    if (Input::isKeyPressed(GLFW_KEY_S))
        m_position -= m_front * velotcity;
    if (Input::isKeyPressed(GLFW_KEY_D))
        m_position += m_right * velotcity;
    if (Input::isKeyPressed(GLFW_KEY_A))
        m_position -= m_right * velotcity;
    if (Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        m_position -= m_worldUp * velotcity;
    if (Input::isKeyPressed(GLFW_KEY_SPACE))
        m_position += m_worldUp * velotcity;

    float xoffset = Input::getMousePosition().x - lastX;
    float yoffset = lastY - Input::getMousePosition().y;
    lastX = Input::getMousePosition().x;
    lastY = Input::getMousePosition().y;

    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    if (first && Input::getMousePosition().x != 0) {
        first = false;
        return;
    }

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    updateVectors();
}

void Camera3D::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(direction);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
