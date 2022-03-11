#include "Chunk.hpp"
#include "Log.hpp"

#include <GLM/gtc/matrix_transform.hpp>
#include <SimplexNoise.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#include <fstream>

// float mapRange(float val, float inMin, float inMax, float outMin, float outMax) {
//     return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
// }

Chunk::Chunk() : m_vao(0), m_nrOfVertices(0), m_nrOfIndices(0), m_loaded(false), m_unload(false) {
    m_blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
}

Chunk::Chunk(const Chunk &other)
    : m_vao(other.m_vao),
      m_nrOfVertices(other.m_nrOfVertices),
      m_nrOfIndices(other.m_nrOfIndices),
      m_loaded(other.m_loaded),
      m_chunkPosition(other.m_chunkPosition) {
    m_blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
    memcpy(m_blocks, other.m_blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT);
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
    model = glm::translate(model, glm::vec3(m_chunkPosition.x * CHUNK_SIZE, m_chunkPosition.y * CHUNK_HEIGHT, m_chunkPosition.z * CHUNK_SIZE));
    shader.setMat4fv("uModel", model, false);

    glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices);
}

void Chunk::generate(int chunkX, int chunkY, int chunkZ, int seed) {
    m_chunkPosition = {chunkX, chunkY, chunkZ};

    SimplexNoise generator(1.0f, 0.5f, 2.0f, 0.5f);

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int index = (z * CHUNK_SIZE * CHUNK_HEIGHT) + (y * CHUNK_SIZE) + x;
                const float incrementSize = 300.f;
                float heightFloat = (generator.fractal(5, (x + ((m_chunkPosition.x * CHUNK_SIZE)) + seed) / incrementSize, (z + ((m_chunkPosition.z * CHUNK_SIZE)) + seed) / incrementSize) + 1) / 2.f;
                int height = (int)(heightFloat * 255);

                m_blocks[index].setBlockType(BlockType::Air);
                int yHeight = y + (chunkY * CHUNK_HEIGHT);

                if (yHeight < height)
                    m_blocks[index].setBlockType(BlockType::Dirt);
                if (yHeight == 0)
                    m_blocks[index].setBlockType(BlockType::Bedrock);
                if (yHeight == height - 1)
                    m_blocks[index].setBlockType(BlockType::Grass);
            }
        }
    }
}

void Chunk::createMesh(TextureAtlas &tex) {
    const int originX = m_chunkPosition.x * CHUNK_SIZE;
    const int originY = m_chunkPosition.y * CHUNK_HEIGHT;
    const int originZ = m_chunkPosition.z * CHUNK_SIZE;

    const int TOTAL_CUBES = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_HEIGHT;
    Vertex *vertices = new Vertex[TOTAL_CUBES * 24];

    const bool lDefault = false;
    int vertexArrayIndex = 0;
    for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
        for (int y = 0; y < Chunk::CHUNK_HEIGHT; y++) {
            for (int x = 0, i = 0; x < Chunk::CHUNK_SIZE; x++) {
                int index = (z * CHUNK_SIZE * CHUNK_HEIGHT) + (y * CHUNK_SIZE) + x;
                if (m_blocks[index].getBlockType() == BlockType::Air)
                    continue;

                std::vector<Vertex> cubeVertices;
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x + 1, y + 1, z + 1), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x + 1, y, z + 1), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x, y, z + 1), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x, y + 1, z + 1), m_blocks[index].getBlockType())});

                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x + 1, y + 1, z), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x + 1, y, z), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x, y, z), m_blocks[index].getBlockType())});
                cubeVertices.emplace_back(Vertex{packData(glm::uvec3(x, y + 1, z), m_blocks[index].getBlockType())});

                bool lXNegative = lDefault;
                if (x > 0) lXNegative = m_blocks[index - 1].isActive();
                bool lXPositive = lDefault;
                if (x < CHUNK_SIZE - 1) lXPositive = m_blocks[index + 1].isActive();
                bool lYNegative = lDefault;
                if (y > 0) lYNegative = m_blocks[index - CHUNK_SIZE].isActive();
                bool lYPositive = lDefault;
                if (y < CHUNK_HEIGHT - 1) lYPositive = m_blocks[index + CHUNK_SIZE].isActive();
                bool lZNegative = lDefault;
                if (z > 0) lZNegative = m_blocks[index - (CHUNK_SIZE * CHUNK_HEIGHT)].isActive();
                bool lZPositive = lDefault;
                if (z < CHUNK_SIZE - 1) lZPositive = m_blocks[index + (CHUNK_SIZE * CHUNK_HEIGHT)].isActive();

                int faceCount = abs((lXPositive + lXNegative + lYPositive + lYNegative + lZPositive + lZNegative) - 6);

                TextureFormat blockFormat = tex.getUVS(m_blocks[index].getBlockType());

                if (!lXNegative)
                    createFace(vertices, vertexArrayIndex, CubeFace::LEFT, cubeVertices, blockFormat);
                if (!lXPositive)
                    createFace(vertices, vertexArrayIndex, CubeFace::RIGHT, cubeVertices, blockFormat);
                if (!lZNegative)
                    createFace(vertices, vertexArrayIndex, CubeFace::BACK, cubeVertices, blockFormat);
                if (!lZPositive)
                    createFace(vertices, vertexArrayIndex, CubeFace::FRONT, cubeVertices, blockFormat);
                if (!lYNegative)
                    createFace(vertices, vertexArrayIndex, CubeFace::BOTTOM, cubeVertices, blockFormat);
                if (!lYPositive)
                    createFace(vertices, vertexArrayIndex, CubeFace::TOP, cubeVertices, blockFormat);
            }
        }
    }
    m_nrOfVertices = vertexArrayIndex;
    uploadToGPU(vertices);
    delete[] vertices;
}

void Chunk::uploadToGPU(Vertex *vertices) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void *)offsetof(Vertex, data));

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
}

uint32_t Chunk::packData(const glm::uvec3 &position, BlockType type) {
    uint32_t data = 0;

    data |= (position.x & 0x1F);
    data |= ((position.y << 5) & 0x3FE0);
    data |= ((position.z << 14) & 0x7C000);
    // data |= ((static_cast<uint16_t>(type) << 19) & 0xFFF0000);
    return data;
}

void Chunk::createFace(Vertex *vertices, int &vertexArrayIndex, const CubeFace face, const std::vector<Vertex> &cubeVertices, const TextureFormat &blockFormat) {
    uint32_t texture = face == CubeFace::TOP      ? blockFormat.top
                       : face == CubeFace::BOTTOM ? blockFormat.bottom
                                                  : blockFormat.side;

    vertices[vertexArrayIndex + 0].data = ((static_cast<int32_t>(face) << 19) & 0x380000);
    vertices[vertexArrayIndex + 1].data = ((static_cast<int32_t>(face) << 19) & 0x380000);
    vertices[vertexArrayIndex + 2].data = ((static_cast<int32_t>(face) << 19) & 0x380000);

    vertices[vertexArrayIndex + 3].data = ((static_cast<int32_t>(face) << 19) & 0x380000);
    vertices[vertexArrayIndex + 4].data = ((static_cast<int32_t>(face) << 19) & 0x380000);
    vertices[vertexArrayIndex + 5].data = ((static_cast<int32_t>(face) << 19) & 0x380000);

    vertices[vertexArrayIndex + 0].data |= (((uint16_t)0 << 22) & 0xC00000);
    vertices[vertexArrayIndex + 1].data |= (((uint16_t)1 << 22) & 0xC00000);
    vertices[vertexArrayIndex + 2].data |= (((uint16_t)2 << 22) & 0xC00000);

    vertices[vertexArrayIndex + 3].data |= (((uint16_t)0 << 22) & 0xC00000);
    vertices[vertexArrayIndex + 4].data |= (((uint16_t)2 << 22) & 0xC00000);
    vertices[vertexArrayIndex + 5].data |= (((uint16_t)3 << 22) & 0xC00000);

    vertices[vertexArrayIndex + 0].data |= ((texture << 24) & 0xFF000000);
    vertices[vertexArrayIndex + 1].data |= ((texture << 24) & 0xFF000000);
    vertices[vertexArrayIndex + 2].data |= ((texture << 24) & 0xFF000000);

    vertices[vertexArrayIndex + 3].data |= ((texture << 24) & 0xFF000000);
    vertices[vertexArrayIndex + 4].data |= ((texture << 24) & 0xFF000000);
    vertices[vertexArrayIndex + 5].data |= ((texture << 24) & 0xFF000000);

    switch (face) {
    case CubeFace::TOP:
        vertices[vertexArrayIndex++].data |= cubeVertices[(1 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(5 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(8 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(1 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(8 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(4 - 1)].data;
        break;
    case CubeFace::BACK:
        vertices[vertexArrayIndex++].data |= cubeVertices[(5 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(6 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(7 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(5 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(7 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(8 - 1)].data;
        break;
    case CubeFace::LEFT:
        vertices[vertexArrayIndex++].data |= cubeVertices[(8 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(7 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(3 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(8 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(3 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(4 - 1)].data;
        break;
    case CubeFace::RIGHT:
        vertices[vertexArrayIndex++].data |= cubeVertices[(1 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(2 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(6 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(1 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(6 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(5 - 1)].data;
        break;
    case CubeFace::FRONT:
        vertices[vertexArrayIndex++].data |= cubeVertices[(4 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(3 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(2 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(4 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(2 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(1 - 1)].data;
        break;
    case CubeFace::BOTTOM:
        vertices[vertexArrayIndex++].data |= cubeVertices[(3 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(7 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(6 - 1)].data;

        vertices[vertexArrayIndex++].data |= cubeVertices[(3 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(6 - 1)].data;
        vertices[vertexArrayIndex++].data |= cubeVertices[(2 - 1)].data;
        break;

    default:
        break;
    }
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

bool Chunk::isSerialized(const std::string &path, const glm::ivec2 &pos) {
    std::string filePath = (path + "/" + std::to_string(pos.x) + "_" + std::to_string(pos.y) + ".bin");
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void Chunk::free() {
    glDeleteVertexArrays(1, &m_vao);
}
