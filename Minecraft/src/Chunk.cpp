#include "Chunk.hpp"

#include <GLM/gtc/matrix_transform.hpp>
#include <SimplexNoise.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include <fstream>

float mapRange(float val, float inMin, float inMax, float outMin, float outMax) {
    return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

Chunk::Chunk() : m_vao(0), m_nrOfVertices(0), m_nrOfIndices(0) {
    m_blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
}

Chunk::~Chunk() {
    delete[] m_blocks;
}

void Chunk::Update(float dt) {
}

void Chunk::Render(Shader &shader) {
    glBindVertexArray(m_vao);
    shader.bind();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_chunkPosition.x * 16, m_chunkPosition.y * 16, m_chunkPosition.z * 16));
    shader.setMat4fv("uModel", model, false);

    glDrawElements(GL_TRIANGLES, m_nrOfIndices, GL_UNSIGNED_INT, 0);
}

void Chunk::generate(int chunkX, int chunkY, int chunkZ, int seed) {
    m_chunkPosition = {chunkX, chunkY, chunkZ};

    SimplexNoise generator(1.0f, 0.5f, 2.0f, 0.5f);

    int chunkIndex = 0;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                const float incrementSize = 300.f;
                float heightFloat = mapRange(generator.fractal(5, (x + ((m_chunkPosition.x * 16)) + seed) / incrementSize, (z + ((m_chunkPosition.z * 16)) + seed) / incrementSize), -1.0f, 1.0f, 0.8f, 1.0f) * (CHUNK_HEIGHT - 1);
                int height = (int)heightFloat;

                m_blocks[chunkIndex].setBlockType(BlockType::Air);

                if (y < height)
                    m_blocks[chunkIndex].setBlockType(BlockType::Dirt);
                if (y == 0)
                    m_blocks[chunkIndex].setBlockType(BlockType::Bedrock);
                if (y == height - 1)
                    m_blocks[chunkIndex].setBlockType(BlockType::Grass);
                chunkIndex++;
            }
        }
    }
}

void Chunk::createMesh(TextureAtlas &tex) {
    const int originX = m_chunkPosition.x * 16;
    const int originY = m_chunkPosition.y * 16;
    const int originZ = m_chunkPosition.z * 16;

    const int TOTAL_CUBES = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_HEIGHT;
    Vertex *vertices = new Vertex[TOTAL_CUBES * 24];
    unsigned int *indices = new unsigned int[TOTAL_CUBES * 36];

    const bool lDefault = false;
    for (int x = 0, i = 0, j = 0, counter = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++, i += 24, j += 36, counter++) {
                if (m_blocks[counter].getBlockType() == BlockType::Air)
                    continue;

                // clang-format off
                Vertex v1 = {setupVertexData(glm::uvec3(x + 1, y + 1, z + 1), m_blocks[counter].getBlockType()), 1.0f, 1.0f};
                Vertex v2 = {setupVertexData(glm::uvec3(x + 1, y, z + 1), m_blocks[counter].getBlockType()), 1.0f, 0.0f};
                Vertex v3 = {setupVertexData(glm::uvec3(x, y, z + 1), m_blocks[counter].getBlockType()), 0.0f, 0.0f};
                Vertex v4 = {setupVertexData(glm::uvec3(x, y + 1, z + 1), m_blocks[counter].getBlockType()), 0.0f, 1.0f};

                Vertex v5 = {setupVertexData(glm::uvec3(x + 1, y + 1, z), m_blocks[counter].getBlockType()), 1.0f, 1.0f};
                Vertex v6 = {setupVertexData(glm::uvec3(x + 1, y, z), m_blocks[counter].getBlockType()), 1.0f, 0.0f};
                Vertex v7 = {setupVertexData(glm::uvec3(x, y, z), m_blocks[counter].getBlockType()), 0.0f, 0.0f};
                Vertex v8 = {setupVertexData(glm::uvec3(x, y + 1, z), m_blocks[counter].getBlockType()) , 0.0f, 1.0f};
                // clang-format on
                bool lXNegative = lDefault;
                if (x > 0) lXNegative = m_blocks[counter - (CHUNK_HEIGHT * CHUNK_SIZE)].isActive();
                bool lXPositive = lDefault;
                if (x < CHUNK_SIZE - 1) lXPositive = m_blocks[counter + (CHUNK_HEIGHT * CHUNK_SIZE)].isActive();
                bool lYNegative = lDefault;
                if (y > 0) lYNegative = m_blocks[counter - CHUNK_SIZE].isActive();
                bool lYPositive = lDefault;
                if (y < CHUNK_HEIGHT - 1) lYPositive = m_blocks[counter + CHUNK_SIZE].isActive();
                bool lZNegative = lDefault;
                if (z > 0) lZNegative = m_blocks[counter - 1].isActive();
                bool lZPositive = lDefault;
                if (z < CHUNK_SIZE - 1) lZPositive = m_blocks[counter + 1].isActive();

                TextureFormat blockFormat = tex.getUVS(m_blocks[counter].getBlockType());

                // Top
                if (!lYPositive) {
                    vertices[i + 0].data |= v1.data;
                    vertices[i + 1].data |= v5.data;
                    vertices[i + 2].data |= v8.data;
                    vertices[i + 3].data |= v4.data;
                }

                // Back
                if (!lZNegative) {
                    vertices[i + 4].data |= v5.data;
                    vertices[i + 5].data |= v6.data;
                    vertices[i + 6].data |= v7.data;
                    vertices[i + 7].data |= v8.data;
                }

                // Left
                if (!lXNegative) {
                    vertices[i + 8].data |= v8.data;
                    vertices[i + 9].data |= v7.data;
                    vertices[i + 10].data |= v3.data;
                    vertices[i + 11].data |= v4.data;
                }

                // Right
                if (!lXPositive) {
                    vertices[i + 12].data |= v1.data;
                    vertices[i + 13].data |= v2.data;
                    vertices[i + 14].data |= v6.data;
                    vertices[i + 15].data |= v5.data;
                }

                // Front
                if (!lZPositive) {
                    vertices[i + 16].data |= v4.data;
                    vertices[i + 17].data |= v3.data;
                    vertices[i + 18].data |= v2.data;
                    vertices[i + 19].data |= v1.data;
                }

                // Bottom
                if (!lYNegative) {
                    vertices[i + 20].data |= v3.data;
                    vertices[i + 21].data |= v7.data;
                    vertices[i + 22].data |= v6.data;
                    vertices[i + 23].data |= v2.data;
                }

                int vertexOffset = counter * 24, elementOffset = counter * 24;
                for (unsigned int i = 0; i < 6; i++) {
                    CubeFace face = static_cast<CubeFace>(i);
                    TextureUVS texture = face == CubeFace::TOP      ? blockFormat.top
                                         : face == CubeFace::BOTTOM ? blockFormat.bottom
                                                                    : blockFormat.side;
                    vertices[vertexOffset + (i * 4) + 0].uv = texture.uvs[0];
                    vertices[vertexOffset + (i * 4) + 1].uv = texture.uvs[1];
                    vertices[vertexOffset + (i * 4) + 2].uv = texture.uvs[2];
                    vertices[vertexOffset + (i * 4) + 3].uv = texture.uvs[3];

                    vertices[vertexOffset + (i * 4) + 0].data |= ((i << 19) & 0x380000);
                    vertices[vertexOffset + (i * 4) + 1].data |= ((i << 19) & 0x380000);
                    vertices[vertexOffset + (i * 4) + 2].data |= ((i << 19) & 0x380000);
                    vertices[vertexOffset + (i * 4) + 3].data |= ((i << 19) & 0x380000);

                    indices[j + (i * 6) + 0] = elementOffset + 0;
                    indices[j + (i * 6) + 1] = elementOffset + 1;
                    indices[j + (i * 6) + 2] = elementOffset + 2;

                    indices[j + (i * 6) + 3] = elementOffset + 0;
                    indices[j + (i * 6) + 4] = elementOffset + 2;
                    indices[j + (i * 6) + 5] = elementOffset + 3;
                    elementOffset += 4;
                }
            }
        }
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (24 * sizeof(Vertex)) * TOTAL_CUBES, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void *)offsetof(Vertex, data));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (36 * sizeof(unsigned int)) * TOTAL_CUBES, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    m_nrOfIndices = 36 * TOTAL_CUBES;
    m_nrOfVertices = 24 * TOTAL_CUBES;

    delete[] vertices;
    delete[] indices;
}

uint32_t Chunk::setupVertexData(const glm::uvec3 &position, BlockType type) {
    uint32_t data = 0;

    data |= (position.x & 0x1F);
    data |= ((position.y << 5) & 0x3FE0);
    data |= ((position.z << 14) & 0x7C000);
    // data |= ((static_cast<uint16_t>(type) << 19) & 0xFFF0000);
    return data;
}

void Chunk::serialize(const std::string &path, int chunkX, int chunkZ) {
    std::string filePath = path + "/" + std::to_string(chunkX) + "_" + std::to_string(chunkZ) + ".bin";
    FILE *fp = std::fopen(filePath.c_str(), "wb");
    if (!fp) {
        std::printf("ERROR::CHUNK_CPP::SERIALIZE::COUDL_NOT_CREATE_FILE -> %s\n", filePath);
    }

    fwrite(&chunkX, sizeof(int), 1, fp);
    fwrite(&chunkZ, sizeof(int), 1, fp);
    fwrite(m_blocks, sizeof(Block) * (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT), 1, fp);
    fclose(fp);
}
void Chunk::deserialize(const std::string &path, int chunkX, int chunkZ) {
    std::string filePath = path + "/" + std::to_string(chunkX) + "_" + std::to_string(chunkZ) + ".bin";
    FILE *fp = std::fopen(filePath.c_str(), "rb");
    if (!fp) {
        std::printf("ERROR::CHUNK_CPP::DESERIALIZE::COUDL_NOT_READ_FILE -> %s\n", filePath.data());
    }

    fread(&m_chunkPosition.x, sizeof(int), 1, fp);
    fread(&m_chunkPosition.z, sizeof(int), 1, fp);
    m_chunkPosition.y = 0;
    fread(m_blocks, sizeof(Block) * (CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT), 1, fp);
    fclose(fp);
}
