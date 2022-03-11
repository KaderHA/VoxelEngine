#pragma once
#include "Camera.hpp"
#include "Chunk.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"

#include <set>
#include <unordered_map>
#include <vector>

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const glm::ivec3 &position) const {
        return (position.x * 88339) ^ (position.z * 91967) ^ (position.z * 126323);
    }
};

class ChunkManager {
public:
    static void init(TextureAtlas &tex);

    static void update(float dt, const Camera3D &camPosition);
    static void render(Shader &shader);

    static void free();

private:
    static void loadChunks(const Camera3D &camera);
    static void unloadChunks();

public:
    static const int CHUNK_RADIUS = 8;
    static const int MAX_LOAD_PER_FRAME = 1;

private:
    static std::unordered_map<glm::ivec3, Chunk, ChunkPositionHash> m_mapChunks;
    static TextureAtlas *m_worldTexture;
};
