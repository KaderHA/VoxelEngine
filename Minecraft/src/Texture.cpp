#include "include/Texture.hpp"

Texture::Texture(BlockType blockType) {
    TextureUVS uvArray[16 * 16];
    const float spacing = 16.f / 256.f;

    for (int y = 16, index = 0; y > 0; y--) {
        for (int x = 0; x < 16; x++, index++) {
            uvArray[index].uvs[0] = {(float)x * spacing, (float)y * spacing};
            uvArray[index].uvs[1] = {(float)x * spacing, (((float)y * spacing) - spacing)};
            uvArray[index].uvs[2] = {((float)x * spacing) + spacing, (((float)y * spacing) - spacing)};
            uvArray[index].uvs[3] = {((float)x * spacing) + spacing, (float)y * spacing};
        }
    }

    // clang-format off
    switch (blockType) {
    case BlockType::Grass:
        m_textureFormat.top = uvArray[0];
        m_textureFormat.side = uvArray[3];
        m_textureFormat.bottom = uvArray[2];
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
        m_textureFormat.top = uvArray[2];
        m_textureFormat.side = uvArray[2];
        m_textureFormat.bottom = uvArray[2];
        break;
    case BlockType::Bedrock:
        m_textureFormat.top.uvs[0] = {0.0625f, 0.9375f};
        m_textureFormat.top.uvs[1] = {0.0625f, 0.9375f - spacing};
        m_textureFormat.top.uvs[2] = {0.0625f + spacing, 0.9375f - spacing};
        m_textureFormat.top.uvs[3] = {0.0625f + spacing, 0.9375f};

        m_textureFormat.side.uvs[0] = {0.0625f, 0.9375f};
        m_textureFormat.side.uvs[1] = {0.0625f, 0.9375f -  spacing};
        m_textureFormat.side.uvs[2] = {0.0625f + spacing, 0.9375f - spacing};
        m_textureFormat.side.uvs[3] = {0.0625f + spacing, 0.9375f};

        m_textureFormat.bottom.uvs[0] = {0.0625f, 0.9375f};
        m_textureFormat.bottom.uvs[1] = {0.0625f, 0.9375f - spacing};
        m_textureFormat.bottom.uvs[2] = {0.0625f + spacing, 0.9375f - spacing};
        m_textureFormat.bottom.uvs[3] = {0.0625f + spacing, 0.9375f};
        break;
    default:
        break;
    }
    //clang-format on
}

Texture::~Texture() {
}
