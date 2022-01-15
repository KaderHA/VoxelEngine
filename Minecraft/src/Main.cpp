#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "Vertex.hpp"
#include "Window.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <random>
#include <time.h>

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main() {

    const int CHUNK_RADIUS = 4;
    srand(static_cast<unsigned int>(time(0)));
    // int seed = (rand() % 1000) + 1;
    int seed = 0;
    Window window(800, 600, "Minecraft!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.48f, 0.74f, 1.0f, 1.0f);

    Shader program("assets/shaders/default.vs", "assets/shaders/default.fs");
    Camera3D cam(glm::vec3(0.0f, Chunk::CHUNK_HEIGHT + 16, 0.0f));
    TextureAtlas textureAtlas("assets/images/terrain.png");

    // Chunk chunk;
    // chunk.generate(0.f, 0.f, 0.0f, seed);
    Chunk chunks[((CHUNK_RADIUS * 2) * (CHUNK_RADIUS * 2))];
    for (int x = cam.getCameraPos().x - CHUNK_RADIUS, i = 0; x < cam.getCameraPos().x + CHUNK_RADIUS; x++) {
        for (int z = cam.getCameraPos().z - CHUNK_RADIUS; z < cam.getCameraPos().z + CHUNK_RADIUS; z++, i++) {
            chunks[i].generate(x, 0, z, seed);
            // chunks[i].serialize("assets/world", x, z);
            // chunks[i].deserialize("assets/world", x, z);
            chunks[i].createMesh(textureAtlas);
        }
    }

    glm::mat4 model(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 255.f);

    program.bind();
    program.setMat4fv("uModel", model);
    program.setMat4fv("uProjection", projection);

    while (window.isOpen()) {
        if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
            window.close(true);
        window.pollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.update(deltaTime);
        window.clear();

        program.bind();
        program.setMat4fv("uView", cam.getViewMatrix());

        program.set1i("uTexture", 0);
        textureAtlas.bind(0);

        // Lighting
        program.setVec3fv("uLight.direction", glm::vec3(0.2f, -1.0f, -0.5f));

        // light properties
        program.setVec3fv("uLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        program.setVec3fv("uLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));

        // chunk.Render(program);
        for (int x = 0, i = 0; x < (CHUNK_RADIUS * 2); x++) {
            for (int z = 0; z < (CHUNK_RADIUS * 2); z++, i++) {
                chunks[i].Render(program);
            }
        }
        window.display();
    }

    return 0;
}
