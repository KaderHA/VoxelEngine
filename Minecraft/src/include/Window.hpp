#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
namespace Minecraft {

class Window {
public:
    Window(int width_, int height_, const char *title_, unsigned int flags = 0);
    ~Window();

    bool isOpen();
    void pollEvents();
    void clear();
    void display();

private:
    bool init(unsigned int flags);

private:
    int m_width, m_height;
    const char *m_title;

    GLFWwindow *m_window;
};
} // namespace Minecraft