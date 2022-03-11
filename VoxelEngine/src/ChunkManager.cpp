#include "ChunkManager.hpp"
#include "Log.hpp"

std::unordered_map<glm::ivec3, Chunk, ChunkPositionHash> ChunkManager::m_mapChunks;
TextureAtlas *ChunkManager::m_worldTexture;

void ChunkManager::init(TextureAtlas &tex) {
    m_worldTexture = &tex;
}

void ChunkManager::update(float dt, const Camera3D &camera) {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        glm::ivec2 camChunkCoords(camera.getCameraPos().x / 16, camera.getCameraPos().z / 16);
        glm::ivec2 chunkSpace(itr->second.getChunkPosition().x - camChunkCoords.x, itr->second.getChunkPosition().z - camChunkCoords.y);

        if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
            itr->second.setUnload(true);
        }
    }

    loadChunks(camera);
    unloadChunks();
}

void ChunkManager::loadChunks(const Camera3D &camera) {
    int nrOfLoad = 0;
    int startZ = (camera.getCameraPos().z / Chunk::CHUNK_SIZE) - CHUNK_RADIUS;
    int endZ = (camera.getCameraPos().z / Chunk::CHUNK_SIZE) + CHUNK_RADIUS;

    int endY = 256 / Chunk::CHUNK_HEIGHT;

    int startX = (camera.getCameraPos().x / Chunk::CHUNK_SIZE) - CHUNK_RADIUS;
    int endX = (camera.getCameraPos().x / Chunk::CHUNK_SIZE) + CHUNK_RADIUS;

    for (int z = startZ, i = 0; z < endZ; z++) {
        if (nrOfLoad == MAX_LOAD_PER_FRAME)
            break;
        for (int y = 0; y < endY; y++) {
            for (int x = startX; x < endX; x++, i++) {
                if (m_mapChunks.find(glm::ivec3(x, y, z)) == m_mapChunks.end() && nrOfLoad <= MAX_LOAD_PER_FRAME) {
                    if (!Chunk::isSerialized("assets/world", glm::ivec2(x, z))) {
                        m_mapChunks.emplace(glm::ivec3(x, y, z), Chunk{}).first->second.generate(x, y, z, 0);
                        // m_mapChunks[glm::ivec3(x, y, z)].serialize("assets/world", x, z);
                    } else
                        m_mapChunks.emplace(glm::ivec3(x, y, z), Chunk{}).first->second.deserialize("assets/world", x, z);

                    m_mapChunks[glm::ivec3(x, y, z)].createMesh(*m_worldTexture);
                    m_mapChunks[glm::ivec3(x, y, z)].setLoaded(true);
                    m_mapChunks[glm::ivec3(x, y, z)].setUnload(false);

                    Log::getLogger()->info("Chunk [{}, {}] loaded", x, z);
                    nrOfLoad++;
                }
            }
        }
    }
}
void ChunkManager::unloadChunks() {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end();) {
        if (itr->second.unload() && itr->second.isLoaded()) {
            Log::getLogger()->info("Chunk [{}, {}] unloaded", itr->second.getChunkPosition().x, itr->second.getChunkPosition().z);
            itr->second.free();
            m_mapChunks.erase(itr++);
        } else
            ++itr;
    }
}

void ChunkManager::render(Shader &shader) {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        if (itr->second.isLoaded() && !itr->second.unload())
            itr->second.Render(shader);
    }
}

void ChunkManager::free() {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        itr->second.free();
    }
    m_mapChunks.clear();
}
