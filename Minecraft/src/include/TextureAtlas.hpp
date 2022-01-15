#pragma once
#include "Block.hpp"
#include "Vertex.hpp"

struct TextureFormat {
    TextureUVS top;
    TextureUVS side;
    TextureUVS bottom;
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
    unsigned int m_ID;
    int m_width, m_height, m_nrChannel;

    TextureUVS *m_uvArray;
    TextureFormat m_textureFormat;
};
