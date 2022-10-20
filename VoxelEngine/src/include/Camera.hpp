#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Input;

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera3D {
public:
    Camera3D(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    void update(float dt);

    // Getters
    glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }
    glm::vec3 getCameraPos() const { return m_position; }

private:
    void updateVectors();

private:
    glm::vec3 m_position;
    glm::vec3 m_front, m_right, m_up, m_worldUp;

    float m_pitch, m_yaw;
    float m_speed, m_sensitivity, m_zoom;
};
