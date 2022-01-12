#pragma once
#include "Block.hpp"
#include "Vertex.hpp"

struct TextureFormat {
    TextureUVS top;
    TextureUVS side;
    TextureUVS bottom;
};

class Texture {
public:
    Texture(BlockType blockType);
    ~Texture();

    TextureFormat getUVS() const { return m_textureFormat; }

private:
    TextureFormat m_textureFormat;
};
