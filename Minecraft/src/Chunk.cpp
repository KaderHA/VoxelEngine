#include "Chunk.hpp"
#include "Texture.hpp"

#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

Chunk::Chunk() : m_vao(0), m_nrOfVertices(0), m_nrOfIndices(0) {
    m_blocks = new Block **[CHUNK_SIZE];
    for (int i = 0; i < CHUNK_SIZE; i++) {
        m_blocks[i] = new Block *[CHUNK_HEIGHT];
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            m_blocks[i][j] = new Block[CHUNK_SIZE];
        }
    }
}

Chunk::~Chunk() {
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_HEIGHT; ++j) {
            delete[] m_blocks[i][j];
        }
        delete[] m_blocks[i];
    }
    delete[] m_blocks;
}

void Chunk::Update(float dt) {
}

void Chunk::Render(Shader &shader) {
    glBindVertexArray(m_vao);

    shader.bind();
    glDrawElements(GL_TRIANGLES, m_nrOfIndices, GL_UNSIGNED_INT, 0);
}

void Chunk::generate(int chunkX, int chunkY, int chunkZ) {
    chunkPosition = {chunkX, chunkY, chunkZ};

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                m_blocks[x][y][z].setBlockType(BlockType::Grass);
                m_blocks[x][y][z].setActive(true);
            }
        }
    }

    createMesh();
}

void Chunk::createMesh() {
    const float originX = chunkPosition.x * 16;
    const float originY = chunkPosition.y * 16;
    const float originZ = chunkPosition.z * 16;

    const int TOTAL_CUBES = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_HEIGHT;
    Vertex *vertices = new Vertex[TOTAL_CUBES * 24];
    unsigned int *indices = new unsigned int[TOTAL_CUBES * 36];

    bool lDefault = false;
    for (int x = 0, i = 0, j = 0, counter = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++, i += 24, j += 36, counter++) {
                if (!m_blocks[x][y][z].isActive())
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
                if (x > 0) lXNegative = m_blocks[x - 1][y][z].isActive();
                bool lXPositive = lDefault;
                if (x < CHUNK_SIZE - 1) lXPositive = m_blocks[x + 1][y][z].isActive();
                bool lYNegative = lDefault;
                if (y > 0) lYNegative = m_blocks[x][y - 1][z].isActive();
                bool lYPositive = lDefault;
                if (y < CHUNK_HEIGHT - 1) lYPositive = m_blocks[x][y + 1][z].isActive();
                bool lZNegative = lDefault;
                if (z > 0) lZNegative = m_blocks[x][y][z - 1].isActive();
                bool lZPositive = lDefault;
                if (z < CHUNK_SIZE - 1) lZPositive = m_blocks[x][y][z + 1].isActive();

                Texture tex(m_blocks[x][y][z].getBlockType());

                int uvIndex = counter * 24;
                for (int i = 0; i < 6; i++) {
                    TextureUVS texture = i == 0 ? tex.getUVS().top : i < 5 ? tex.getUVS().side
                                                                           : tex.getUVS().bottom;
                    vertices[uvIndex + (i * 4) + 0].uv = texture.uvs[0];
                    vertices[uvIndex + (i * 4) + 1].uv = texture.uvs[1];
                    vertices[uvIndex + (i * 4) + 2].uv = texture.uvs[2];
                    vertices[uvIndex + (i * 4) + 3].uv = texture.uvs[3];
                }

                // Front
                if (!lZPositive) {
                    vertices[i + 16].position = v4.position;
                    vertices[i + 17].position = v3.position;
                    vertices[i + 18].position = v2.position;
                    vertices[i + 19].position = v1.position;
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

                // Top
                if (!lYPositive) {
                    vertices[i + 0].position = v1.position;
                    vertices[i + 1].position = v5.position;
                    vertices[i + 2].position = v8.position;
                    vertices[i + 3].position = v4.position;
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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));

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
