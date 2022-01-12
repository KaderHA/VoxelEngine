#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Window.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main() {
    Window window(800, 600, "Minecraft!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.48f, 0.74f, 1.0f, 1.0f);

    Shader program("assets/shaders/default.vs", "assets/shaders/default.fs");
    Camera3D cam(glm::vec3(0.0f, 272.0f, 0.0f));

    // Chunk chunk;
    // chunk.generate(heightMap);
    // chunk.createMesh(0.f, 0.f, 256.f);
    Chunk chunks[16];
    for (int x = cam.getCameraPos().x - 2, i = 0; x < cam.getCameraPos().x + 2; x++) {
        for (int z = cam.getCameraPos().z - 2; z < cam.getCameraPos().z + 2; z++, i++) {
            chunks[i].generate(x, 0, z);
        }
    }

    glm::mat4 model(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);

    program.bind();
    program.setMat4fv("uModel", model);
    program.setMat4fv("uProjection", projection);

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/images/terrain_atlas.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << '\n';
    }
    stbi_image_free(data);

    while (window.isOpen()) {
        if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) window.close(true);
        window.pollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.update(deltaTime);
        window.clear();

        program.bind();
        program.setMat4fv("uView", cam.getViewMatrix());

        program.set1i("uTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);

        // chunk.Render(program);
        for (int x = 0, i = 0; x < 4; x++) {
            for (int z = 0; z < 4; z++, i++) {
                chunks[i].Render(program);
            }
        }
        window.display();
    }

    return 0;
}
