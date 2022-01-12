#include "Texture.hpp"

Texture::Texture(BlockType blockType) {
    // clang-format off
    const float spacing = 0.0625f;
    switch (blockType) {
    case BlockType::Grass:
        m_textureFormat.top.uvs[0] = {0.5f, 0.8125f + spacing};
        m_textureFormat.top.uvs[1] = {0.5f, 0.8125f};
        m_textureFormat.top.uvs[2] = {0.5f + spacing,   0.8125f};
        m_textureFormat.top.uvs[3] = {0.5f + spacing,   0.8125f + spacing}; 

        m_textureFormat.side.uvs[0] = {0.1875f, 1.0f};
        m_textureFormat.side.uvs[1] = {0.1875f, 0.9375f};
        m_textureFormat.side.uvs[2] = {0.1875f + spacing, 0.9375f};
        m_textureFormat.side.uvs[3] = {0.1875f + spacing, 1.0f};  

        m_textureFormat.bottom.uvs[0] = {0.125f, 1.0f};
        m_textureFormat.bottom.uvs[1] = {0.125f, 0.9375f};
        m_textureFormat.bottom.uvs[2] = {0.125f + spacing, 0.9375f};
        m_textureFormat.bottom.uvs[3] = {0.125f + spacing, 1.0f};
        break;
    case BlockType::Dirt:
        m_textureFormat.top.uvs[0] = {0.125f, 1.0f};
        m_textureFormat.top.uvs[1] = {0.125f, 0.9375f};
        m_textureFormat.top.uvs[2] = {0.125f + spacing, 0.9375f};
        m_textureFormat.top.uvs[3] = {0.125f + spacing, 1.0f};

        m_textureFormat.side.uvs[0] = {0.125f, 1.0f};
        m_textureFormat.side.uvs[1] = {0.125f, 0.9375f};
        m_textureFormat.side.uvs[2] = {0.125f + spacing, 0.9375f};
        m_textureFormat.side.uvs[3] = {0.125f + spacing, 1.0f};

        m_textureFormat.bottom.uvs[0] = {0.125f, 1.0f};
        m_textureFormat.bottom.uvs[1] = {0.125f, 0.9375f};
        m_textureFormat.bottom.uvs[2] = {0.125f + spacing, 0.9375f};
        m_textureFormat.bottom.uvs[3] = {0.125f + spacing, 1.0f};
        break;
    case BlockType::Stone:
        m_textureFormat.top.uvs[0] = {0.0625f, 1.0f};
        m_textureFormat.top.uvs[1] = {0.0625f, 0.9375f};
        m_textureFormat.top.uvs[2] = {0.0625f + spacing, 0.9375f};
        m_textureFormat.top.uvs[3] = {0.0625f + spacing, 1.0f};

        m_textureFormat.side.uvs[0] = {0.0625f, 1.0f};
        m_textureFormat.side.uvs[1] = {0.0625f, 0.9375f};
        m_textureFormat.side.uvs[2] = {0.0625f + spacing, 0.9375f};
        m_textureFormat.side.uvs[3] = {0.0625f + spacing, 1.0f};

        m_textureFormat.bottom.uvs[0] = {0.0625f, 1.0f};
        m_textureFormat.bottom.uvs[1] = {0.0625f, 0.9375f};
        m_textureFormat.bottom.uvs[2] = {0.0625f + spacing, 0.9375f};
        m_textureFormat.bottom.uvs[3] = {0.0625f + spacing, 1.0f};
        break;
    default:
        break;
    }
    //clang-format on
}

Texture::~Texture() {
}
