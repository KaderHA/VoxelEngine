#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include "ChunkManager.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Vertex.hpp"
#include "Window.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <random>
#include <time.h>

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
bool wireframe = false;

int main() {

    Log::init();
    Log::getLogger()->error("Logger initialized");
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
    ChunkManager chunks;
    chunks.init();

    glm::mat4 model(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(90.0f), 800.f / 600.f, 0.1f, 255.f);

    program.bind();
    program.setMat4fv("uModel", model);
    program.setMat4fv("uProjection", projection);

    while (window.isOpen()) {
        if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
            window.close(true);
        if (Input::isKeySinglePress(GLFW_KEY_V)) {
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, (GL_FILL - wireframe));
        }
        window.pollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cam.update(deltaTime);
        window.clear();

        program.bind();
        program.setMat4fv("uView", cam.getViewMatrix());

        program.set1i("uTexture", 0);
        program.set1i("uTexUV", 1);
        textureAtlas.bind(GL_TEXTURE0);

        // Lighting
        program.setVec3fv("uLight.direction", glm::vec3(0.2f, -1.0f, -0.5f));

        // light properties
        program.setVec3fv("uLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        program.setVec3fv("uLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));

        chunks.update(deltaTime, cam, textureAtlas);
        chunks.render(program);
        window.display();
    }

    return 0;
}
