#pragma once
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentShader, const char *geometryFile = nullptr);
    ~Shader();

    void bind();
    void unbind();

    void set1i(const char *name, int value);
    void set1f(const char *name, float value);
    void setVec2fv(const char *name, glm::vec2 value);
    void setVec3fv(const char *name, glm::vec3 value);
    void setVec4fv(const char *name, glm::vec4 value);
    void setMat4fv(const char *name, glm::mat4 value, bool transpose = false);

private:
    int getLoc(const char *name);
    std::string loadShaderSource(const char *filename);
    unsigned int loadShader(unsigned int type, const char *filename);
    void linkProgram(unsigned int vShader, unsigned int fShader, unsigned int gShader);

private:
    unsigned int m_ID;
};
