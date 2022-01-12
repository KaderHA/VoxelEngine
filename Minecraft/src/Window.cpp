#include "Window.hpp"
#include <iostream>

namespace Minecraft {
Window::Window(int width_, int height_, const char *title_, unsigned int flags)
    : m_width(width_), m_height(height_), m_title(title_) {
    if (!init(flags)) {
        std::cout << "Failed to initialize!" << std::endl;
    }
}

Window::~Window() {
}

bool Window::init(unsigned int flags) {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, m_title, 0, 0);
    if (!m_window) {
        return false;
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
    return true;
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::display() {
    glfwSwapBuffers(m_window);
}

} // namespace Minecraft