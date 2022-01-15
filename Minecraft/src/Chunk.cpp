#include "Chunk.hpp"
#include "Texture.hpp"

#include <SimplexNoise.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include <fstream>

float mapRange(float val, float inMin, float inMax, float outMin, float outMax) {
    return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

Vertex::Normal getNormal(CubeFace face) {
    if (face == CubeFace::TOP) return {0.0f, 1.0f, 0.0f};
    if (face == CubeFace::BACK) return {0.0f, 0.0f, -1.0f};
    if (face == CubeFace::LEFT) return {-1.0f, 0.0f, 0.0f};
    if (face == CubeFace::RIGHT) return {1.0f, 0.0f, 0.0f};
    if (face == CubeFace::FRONT) return {0.0f, 0.0f, 1.0f};
    if (face == CubeFace::BOTTOM) return {0.0f, -1.0f, 0.0f};
    return {0.0f, 0.0f, 0.0f};
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
                float heightFloat = mapRange(generator.fractal(5, (x + ((m_chunkPosition.x * 16)) + seed) / incrementSize, (z + ((m_chunkPosition.z * 16)) + seed) / incrementSize), -1.0f, 1.0f, 0.1f, 1.0f) * (CHUNK_HEIGHT - 1);
                int height = (int)heightFloat;

                m_blocks[chunkIndex].setBlockType(BlockType::Air);

                if (y < height)
                    m_blocks[chunkIndex].setBlockType(BlockType::Dirt);
                if (y == height - 1)
                    m_blocks[chunkIndex].setBlockType(BlockType::Grass);
                chunkIndex++;
            }
        }
    }
}

void Chunk::createMesh() {
    const float originX = m_chunkPosition.x * 16;
    const float originY = m_chunkPosition.y * 16;
    const float originZ = m_chunkPosition.z * 16;

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
                Vertex v1 = {( 0.5f + x) + originX, ( 0.5f + y) + originY, ( 0.5f + z) + originZ, 1.0f, 1.0f};
                Vertex v2 = {( 0.5f + x) + originX, (-0.5f + y) + originY, ( 0.5f + z) + originZ, 1.0f, 0.0f};
                Vertex v3 = {(-0.5f + x) + originX, (-0.5f + y) + originY, ( 0.5f + z) + originZ, 0.0f, 0.0f};
                Vertex v4 = {(-0.5f + x) + originX, ( 0.5f + y) + originY, ( 0.5f + z) + originZ, 0.0f, 1.0f};

                Vertex v5 = {( 0.5f + x) + originX, ( 0.5f + y) + originY, (-0.5f + z) + originZ, 1.0f, 1.0f};
                Vertex v6 = {( 0.5f + x) + originX, (-0.5f + y) + originY, (-0.5f + z) + originZ, 1.0f, 0.0f};
                Vertex v7 = {(-0.5f + x) + originX, (-0.5f + y) + originY, (-0.5f + z) + originZ, 0.0f, 0.0f};
                Vertex v8 = {(-0.5f + x) + originX, ( 0.5f + y) + originY, (-0.5f + z) + originZ, 0.0f, 1.0f};
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

                Texture tex(m_blocks[counter].getBlockType());

                int vertexOffset = counter * 24;
                for (int i = 0; i < 6; i++) {
                    CubeFace face = static_cast<CubeFace>(i);
                    TextureUVS texture = face == CubeFace::TOP      ? tex.getUVS().top
                                         : face == CubeFace::BOTTOM ? tex.getUVS().bottom
                                                                    : tex.getUVS().side;
                    vertices[vertexOffset + (i * 4) + 0].uv = texture.uvs[0];
                    vertices[vertexOffset + (i * 4) + 1].uv = texture.uvs[1];
                    vertices[vertexOffset + (i * 4) + 2].uv = texture.uvs[2];
                    vertices[vertexOffset + (i * 4) + 3].uv = texture.uvs[3];

                    Vertex::Normal normal = getNormal(static_cast<CubeFace>(i));
                    vertices[vertexOffset + (i * 4) + 0].normal = normal;
                    vertices[vertexOffset + (i * 4) + 1].normal = normal;
                    vertices[vertexOffset + (i * 4) + 2].normal = normal;
                    vertices[vertexOffset + (i * 4) + 3].normal = normal;
                }

                // Top
                if (!lYPositive) {
                    vertices[i + 0].position = v1.position;
                    vertices[i + 1].position = v5.position;
                    vertices[i + 2].position = v8.position;
                    vertices[i + 3].position = v4.position;
                }

                // Back
                if (!lZNegative) {
                    vertices[i + 4].position = v5.position;
                    vertices[i + 5].position = v6.position;
                    vertices[i + 6].position = v7.position;
                    vertices[i + 7].position = v8.position;
                }

                // Left
                if (!lXNegative) {
                    vertices[i + 8].position = v8.position;
                    vertices[i + 9].position = v7.position;
                    vertices[i + 10].position = v3.position;
                    vertices[i + 11].position = v4.position;
                }

                // Right
                if (!lXPositive) {
                    vertices[i + 12].position = v1.position;
                    vertices[i + 13].position = v2.position;
                    vertices[i + 14].position = v6.position;
                    vertices[i + 15].position = v5.position;
                }

                // Front
                if (!lZPositive) {
                    vertices[i + 16].position = v4.position;
                    vertices[i + 17].position = v3.position;
                    vertices[i + 18].position = v2.position;
                    vertices[i + 19].position = v1.position;
                }

                // Bottom
                if (!lYNegative) {
                    vertices[i + 20].position = v3.position;
                    vertices[i + 21].position = v7.position;
                    vertices[i + 22].position = v6.position;
                    vertices[i + 23].position = v2.position;
                }

                int elementOffset = counter * 24;
                for (int k = 0; k < 6; k++) {
                    indices[j + (k * 6) + 0] = elementOffset + 0;
                    indices[j + (k * 6) + 1] = elementOffset + 1;
                    indices[j + (k * 6) + 2] = elementOffset + 2;

                    indices[j + (k * 6) + 3] = elementOffset + 0;
                    indices[j + (k * 6) + 4] = elementOffset + 2;
                    indices[j + (k * 6) + 5] = elementOffset + 3;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, uv)));

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
