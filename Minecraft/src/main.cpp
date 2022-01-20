#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Input.hpp"
#include "Log.hpp"
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
const int CHUNK_RADIUS = 4;
const int MAX_LOAD_PER_FRAME = 1;
bool wireframe = false;

void loadChunks(std::vector<Chunk> &chunks, std::set<std::pair<int, int>> &loaded, const Camera3D &cam, TextureAtlas &tex);
// void unloadChunks(std::vector<Chunk> &chunks);

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
    std::vector<Chunk> chunks;
    std::set<std::pair<int, int>> loaded;

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

        // chunk.Render(program);
        for (int i = 0; i < chunks.size(); i++) {
            chunks[i].Render(program);

            glm::ivec2 camChunkCoords(cam.getCameraPos().x / 16, cam.getCameraPos().z / 16);
            glm::ivec2 chunkSpace(chunks[i].getChunkPosition().x - camChunkCoords.x, chunks[i].getChunkPosition().z - camChunkCoords.y);
            if (((chunkSpace.x * chunkSpace.x) + (chunkSpace.y * chunkSpace.y)) >= (CHUNK_RADIUS * 2) * (CHUNK_RADIUS * 2)) {
                chunks[i].setLoaded(false);
                loaded.erase(std::make_pair(chunks[i].getChunkPosition().x, chunks[i].getChunkPosition().z));
            }
        }

        // unloadChunks(chunks);
        loadChunks(chunks, loaded, cam, textureAtlas);
        window.display();
    }

    return 0;
}

void loadChunks(std::vector<Chunk> &chunks, std::set<std::pair<int, int>> &loaded, const Camera3D &cam, TextureAtlas &tex) {
    int nrOfLoad = 0;
    for (int x = (cam.getCameraPos().x / 16) - CHUNK_RADIUS, i = 0; x < (cam.getCameraPos().x / 16) + CHUNK_RADIUS; x++) {
        for (int z = (cam.getCameraPos().z / 16) - CHUNK_RADIUS; z < (cam.getCameraPos().z / 16) + CHUNK_RADIUS; z++, i++) {
            if (loaded.find(std::make_pair(x, z)) == loaded.end() && nrOfLoad <= MAX_LOAD_PER_FRAME) {
                Chunk chunk;
                chunk.generate(x, 0, z, 0);
                chunk.createMesh(tex);
                chunks.emplace_back(chunk);
                loaded.insert(std::make_pair(x, z));
                nrOfLoad++;
            }
        }
    }
}

// void unloadChunks(std::vector<Chunk> &chunks) {
//     for (auto itr = chunks.begin(); itr != chunks.end();) {
//         if (!itr->isLoaded()) {
//             itr->free();
//             itr = chunks.erase(itr);
//         } else
//             itr++;
//     }
// }