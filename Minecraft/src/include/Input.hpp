#pragma once
#include <GLAD/glad.h>

#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

class Input {
    friend class Window;

public:
    static bool isKeyPressed(int key);
    static bool isButtonPressed(int button);
    static glm::vec2 getMousePosition() { return m_mousePos; }

    static bool isKeySinglePress(int key);

private:
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

private:
    static bool m_keys[GLFW_KEY_LAST];
    static bool m_buttons[GLFW_MOUSE_BUTTON_LAST];
    static glm::vec2 m_mousePos;
};
