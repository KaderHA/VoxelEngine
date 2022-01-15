#pragma once
#include "Block.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "Vertex.hpp"

#include <glm/glm.hpp>
#include <string>

enum class CubeFace {
    TOP = 0,
    BACK,
    LEFT,
    RIGHT,
    FRONT,
    BOTTOM
};

class Chunk {
public:
    Chunk();
    ~Chunk();

    void Update(float dt);
    void Render(Shader &shader);

    void generate(int chunkX = 0, int chunkY = 0, int chunkZ = 0, int seed = 0);
    void createMesh(TextureAtlas &tex);

    void serialize(const std::string &path, int chunkX, int chunkZ);
    void deserialize(const std::string &path, int chunkX, int chunkZ);

    glm::vec3 getChunkPosition() const { return m_chunkPosition; }

public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 256;

private:
    unsigned int m_vao;
    int m_nrOfVertices, m_nrOfIndices;

    glm::ivec3 m_chunkPosition;
    Block *m_blocks;
};