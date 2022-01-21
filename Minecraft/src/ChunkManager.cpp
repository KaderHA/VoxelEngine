#include "ChunkManager.hpp"
#include "Log.hpp"

std::unordered_map<glm::ivec3, Chunk, ChunkPositionHash> ChunkManager::m_mapChunks;

void ChunkManager::init() {
}

void ChunkManager::update(float dt, const Camera3D &camera, TextureAtlas &chunkTexture) {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        glm::ivec2 camChunkCoords(camera.getCameraPos().x / 16, camera.getCameraPos().z / 16);
        glm::ivec2 chunkSpace(itr->second.getChunkPosition().x - camChunkCoords.x, itr->second.getChunkPosition().z - camChunkCoords.y);

        if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
            itr->second.setLoaded(false);
        }
    }

    loadChunks(camera, chunkTexture);
    unloadChunks();
}

void ChunkManager::loadChunks(const Camera3D &camera, TextureAtlas &tex) {
    int nrOfLoad = 0;
    for (int x = (camera.getCameraPos().x / 16) - CHUNK_RADIUS, i = 0; x < (camera.getCameraPos().x / 16) + CHUNK_RADIUS; x++) {
        for (int z = (camera.getCameraPos().z / 16) - CHUNK_RADIUS; z < (camera.getCameraPos().z / 16) + CHUNK_RADIUS; z++, i++) {
            if (m_mapChunks.find(glm::ivec3(x, 0, z)) == m_mapChunks.end() && nrOfLoad <= MAX_LOAD_PER_FRAME) {
                if (!Chunk::isSerialized("assets/world", glm::ivec2(x, z))) {
                    m_mapChunks.emplace(glm::ivec3(x, 0, z), Chunk{}).first->second.generate(x, 0, z, 0);
                    m_mapChunks.emplace(glm::ivec3(x, 0, z), Chunk{}).first->second.serialize("assets/world", x, z);
                } else
                    m_mapChunks.emplace(glm::ivec3(x, 0, z), Chunk{}).first->second.deserialize("assets/world", x, z);

                m_mapChunks[glm::ivec3(x, 0, z)].createMesh(tex);
                nrOfLoad++;
            }
        }
    }
}
void ChunkManager::unloadChunks() {
    for (auto itr = m_mapChunks.begin(), nextItr = itr; itr != m_mapChunks.end(); itr = nextItr) {
        ++nextItr;
        if (!itr->second.isLoaded()) {
            Log::getLogger()->info("Chunk {}, {} unloaded", itr->second.getChunkPosition().x, itr->second.getChunkPosition().z);
            itr->second.free();
            m_mapChunks.erase(itr->first);
        }
    }
}

void ChunkManager::render(Shader &shader) {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        itr->second.Render(shader);
    }
}

void ChunkManager::free() {
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        itr->second.free();
    }
    m_mapChunks.clear();
}
