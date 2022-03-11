#pragma once
#include "Block.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "Vertex.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

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
    Chunk(const Chunk &other);

    ~Chunk();

    void Update(float dt);
    void Render(Shader &shader);

    void generate(int chunkX = 0, int chunkY = 0, int chunkZ = 0, int seed = 0);
    void createMesh(TextureAtlas &tex);
    void uploadToGPU(Vertex *vertices);

    void serialize(const std::string &path, int chunkX, int chunkZ);
    void deserialize(const std::string &path, int chunkX, int chunkZ);

    void free();

    // Gettesr & setters
    glm::ivec3 getChunkPosition() const { return m_chunkPosition; }

    bool isLoaded() const { return m_loaded; }
    void setLoaded(bool value) { m_loaded = value; }

    bool unload() const { return m_unload; }
    void setUnload(bool value) { m_unload = value; }

    static bool isSerialized(const std::string &path, const glm::ivec2 &pos);

private:
    void createFace(Vertex *vertices, int &vertexArrayIndex, const CubeFace face, const std::vector<Vertex> &cubeVertices, const TextureFormat &blockFormat);
    uint32_t packData(const glm::uvec3 &position, BlockType type);
    bool calculateChunkNeighbor(const glm::ivec3 &pos, bool &nx, bool &px, bool &ny, bool &py, bool &nz, bool &pz);

public:
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 256;

private:
    unsigned int m_vao;
    int m_nrOfVertices, m_nrOfIndices;
    bool m_loaded, m_unload;

    glm::ivec3 m_chunkPosition;
    Block *m_blocks;
};