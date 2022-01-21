#include "ChunkManager.hpp"

std::unordered_map<glm::ivec3, Chunk, ChunkPositionHash> ChunkManager::m_mapChunks;
// std::vector<Chunk *> ChunkManager::m_chunks;
// std::set<std::pair<int, int>> ChunkManager::m_loaded;

void ChunkManager::init() {
}

void ChunkManager::update(float dt, const Camera3D &camera, TextureAtlas &chunkTexture) {
    // for (int i = 0; i < m_chunks.size(); i++) {
    //     glm::ivec2 camChunkCoords(camera.getCameraPos().x / 16, camera.getCameraPos().z / 16);
    //     glm::ivec2 chunkSpace(m_chunks[i]->getChunkPosition().x - camChunkCoords.x, m_chunks[i]->getChunkPosition().z - camChunkCoords.y);

    //     if (abs(chunkSpace.x) > CHUNK_RADIUS || abs(chunkSpace.y) > CHUNK_RADIUS) {
    //         m_chunks[i]->setLoaded(false);
    //         m_loaded.erase(std::make_pair(m_chunks[i]->getChunkPosition().x, m_chunks[i]->getChunkPosition().z));
    //     }
    // }

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
    // for (int x = (camera.getCameraPos().x / 16) - CHUNK_RADIUS, i = 0; x < (camera.getCameraPos().x / 16) + CHUNK_RADIUS; x++) {
    //     for (int z = (camera.getCameraPos().z / 16) - CHUNK_RADIUS; z < (camera.getCameraPos().z / 16) + CHUNK_RADIUS; z++, i++) {
    //         if (m_loaded.find(std::make_pair(x, z)) == m_loaded.end() && nrOfLoad <= MAX_LOAD_PER_FRAME) {
    //             Chunk *chunk = new Chunk;
    //             chunk->generate(x, 0, z, 0);
    //             chunk->createMesh(tex);
    //             m_chunks.push_back(chunk);
    //             m_loaded.insert(std::make_pair(x, z));
    //             nrOfLoad++;
    //         }
    //     }
    // }
    for (int x = (camera.getCameraPos().x / 16) - CHUNK_RADIUS, i = 0; x < (camera.getCameraPos().x / 16) + CHUNK_RADIUS; x++) {
        for (int z = (camera.getCameraPos().z / 16) - CHUNK_RADIUS; z < (camera.getCameraPos().z / 16) + CHUNK_RADIUS; z++, i++) {
            if (m_mapChunks.find(glm::ivec3(x, 0, z)) == m_mapChunks.end() && nrOfLoad <= MAX_LOAD_PER_FRAME) {
                m_mapChunks.emplace(glm::ivec3(x, 0, z), Chunk{}).first->second.generate(x, 0, z, 0);
                m_mapChunks[glm::ivec3(x, 0, z)].createMesh(tex);
                nrOfLoad++;
            }
        }
    }
}
void ChunkManager::unloadChunks() {
    // for (auto itr = m_chunks.begin(); itr != m_chunks.end();) {
    //     if (!(*itr)->isLoaded()) {
    //         (*itr)->free();
    //         delete *itr;
    //         itr = m_chunks.erase(itr);
    //     } else
    //         itr++;
    // }

    for (auto itr = m_mapChunks.begin(), nextItr = itr; itr != m_mapChunks.end(); itr = nextItr) {
        ++nextItr;
        if (!itr->second.isLoaded()) {
            itr->second.free();
            m_mapChunks.erase(itr->first);
        }
    }
}

void ChunkManager::render(Shader &shader) {
    // for (int i = 0; i < m_chunks.size(); i++) {
    //     m_chunks[i]->Render(shader);
    // }
    for (auto itr = m_mapChunks.begin(); itr != m_mapChunks.end(); ++itr) {
        itr->second.Render(shader);
    }
}

void ChunkManager::free() {
    // for (int i = 0; i < m_chunks.size(); i++) {
    //     m_chunks[i]->free();
    //     delete m_chunks[i];
    // }

    m_mapChunks.clear();
}
