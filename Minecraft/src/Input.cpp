#include "Input.hpp"

bool Input::m_keys[GLFW_KEY_LAST];
bool Input::m_buttons[GLFW_MOUSE_BUTTON_LAST];
glm::vec2 Input::m_mousePos;

bool Input::isKeyPressed(int key) {
    if (key >= GLFW_KEY_LAST)
        return false;
    return m_keys[key];
}

bool Input::isButtonPressed(int button) {
    if (button >= GLFW_MOUSE_BUTTON_LAST)
        return false;
    return m_buttons[button];
}

void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    m_keys[key] = action != GLFW_RELEASE;
}

void Input::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    m_buttons[button] = action != GLFW_RELEASE;
}

void Input::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    m_mousePos.x = xpos;
    m_mousePos.y = ypos;
}

void Input::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
