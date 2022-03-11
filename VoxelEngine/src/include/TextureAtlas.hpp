#pragma once
#include "Block.hpp"
#include "Vertex.hpp"

struct TextureFormat {
    uint32_t top;
    uint32_t side;
    uint32_t bottom;
};

class TextureAtlas {
public:
    TextureAtlas(const char *filePath);
    ~TextureAtlas();

    void bind(const int textureUnit);
    void unbind();

    void generateUVS();
    TextureFormat getUVS(BlockType type);

private:
    unsigned int m_ID, m_uvID, m_tempID;
    int m_width, m_height, m_nrChannel;

    float *m_uvArray;
    TextureFormat m_textureFormat;
};
