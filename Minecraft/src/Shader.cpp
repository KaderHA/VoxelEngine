#include "Shader.hpp"

#include <glad/glad.h>

#include <fstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    unsigned int vShader = 0, fShader = 0, gShader = 0;
    vShader = loadShader(GL_VERTEX_SHADER, vertexPath);
    fShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
    if (geometryPath)
        gShader = loadShader(GL_GEOMETRY_SHADER, geometryPath);

    linkProgram(vShader, fShader, gShader);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

void Shader::bind() {
    glUseProgram(m_ID);
}
void Shader::unbind() {
    glUseProgram(0);
}

void Shader::set1i(const char *name, int value) {
    glUniform1i(getLoc(name), value);
}

void Shader::set1f(const char *name, float value) {
    glUniform1f(getLoc(name), value);
}

void Shader::setVec2fv(const char *name, glm::vec2 value) {
    glUniform2fv(getLoc(name), 1, &value[0]);
}

void Shader::setVec3fv(const char *name, glm::vec3 value) {
    glUniform3fv(getLoc(name), 1, &value[0]);
}

void Shader::setVec4fv(const char *name, glm::vec4 value) {
    glUniform4fv(getLoc(name), 1, &value[0]);
}

void Shader::setMat4fv(const char *name, glm::mat4 value, bool transpose) {
    glUniformMatrix4fv(getLoc(name), 1, transpose, &value[0][0]);
}

int Shader::getLoc(const char *name) {
    return glGetUniformLocation(m_ID, name);
}

std::string Shader::loadShaderSource(const char *filename) {
    std::FILE *fp = std::fopen(filename, "rb");
    if (fp) {
        std::string contents;
        std::fseek(fp, 0, SEEK_END);
        contents.resize(std::ftell(fp));
        std::rewind(fp);
        std::fread(&contents[0], 1, contents.size(), fp);
        std::fclose(fp);
        return contents;
    } else {
        std::printf("ERROR::SHADER_CPP::LOADSHADERSOURCE::COUDL_NOT_OPEN_FILE -> %s\n", filename);
    }
    return nullptr;
}

unsigned int Shader::loadShader(unsigned int type, const char *filename) {
    int success;
    char infoLog[512];

    unsigned int shader = glCreateShader(type);
    std::string str_src = loadShaderSource(filename).c_str();
    const char *source = str_src.c_str();

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        const char *shaderType = type == GL_VERTEX_SHADER ? "VERTEX_SHADER" : type == GL_FRAGMENT_SHADER ? "FRAGMENT_SHADER"
                                                                                                         : "GEOMETRY_SHADER";
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::printf("ERROR::LOADSHADERS::COULD_NOT_COMPILE_%s", shaderType);
        std::printf("%s\n", infoLog);
        return 0;
    }
    return shader;
}

void Shader::linkProgram(unsigned int vShader, unsigned int fShader, unsigned int gShader) {
    int success;
    char infoLog[512];

    m_ID = glCreateProgram();

    glAttachShader(m_ID, vShader);
    glAttachShader(m_ID, fShader);
    if (gShader)
        glAttachShader(m_ID, gShader);

    glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::printf("ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM\n");
        std::printf("%s", infoLog);
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(gShader);
    glDeleteShader(fShader);
}
