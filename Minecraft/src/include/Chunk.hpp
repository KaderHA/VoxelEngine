#pragma once
#include "Block.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include <glm/glm.hpp>

namespace noise::utils {
class NoiseMap;
}

class Chunk {
public:
    Chunk();
    ~Chunk();

    void Update(float dt);
    void Render(Shader &shader);

    void generate(int chunkX = 0, int chunkY = 0, int chunkZ = 0);
    void createMesh();

public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 256;

private:
    unsigned int m_vao;
    int m_nrOfVertices, m_nrOfIndices;

    glm::ivec3 chunkPosition;
    Block ***m_blocks;
};