#include "TextureAtlas.hpp"

#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>

TextureAtlas::TextureAtlas(const char *filePath) {

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(filePath, &m_width, &m_height, &m_nrChannel, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << '\n';
    }
    stbi_image_free(data);

    generateUVS();
}

TextureAtlas::~TextureAtlas() {
    glDeleteTextures(1, &m_ID);

    delete[] m_uvArray;
}

void TextureAtlas::bind(const int textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glActiveTexture(textureUnit + 1);
    glBindTexture(GL_TEXTURE_BUFFER, m_uvID);
}
void TextureAtlas::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::generateUVS() {
    const int textureSize = 16;
    m_uvArray = new float[((m_width / textureSize) * (m_height / textureSize)) * 8];
    const float spacing = 16.f / 256.f;

    for (int y = 16, index = 0; y > 0; y--) {
        for (int x = 0; x < 16; x++, index += 8) {
            m_uvArray[index + 0] = (float)x * spacing;
            m_uvArray[index + 1] = (float)y * spacing;

            m_uvArray[index + 2] = (float)x * spacing;
            m_uvArray[index + 3] = (((float)y * spacing) - spacing);

            m_uvArray[index + 4] = ((float)x * spacing) + spacing;
            m_uvArray[index + 5] = (((float)y * spacing) - spacing);

            m_uvArray[index + 6] = ((float)x * spacing) + spacing;
            m_uvArray[index + 7] = (float)y * spacing;
        }
    }

    glGenBuffers(1, &m_tempID);
    glBindBuffer(GL_TEXTURE_BUFFER, m_tempID);
    glBufferData(GL_TEXTURE_BUFFER, (m_width / textureSize) * (m_height / textureSize) * (sizeof(float) * 8), m_uvArray, GL_STATIC_DRAW);

    glGenTextures(1, &m_uvID);
    glBindTexture(GL_TEXTURE_BUFFER, m_uvID);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_tempID);

    glBindTexture(GL_TEXTURE_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
    glDeleteBuffers(1, &m_tempID);
}

TextureFormat TextureAtlas::getUVS(BlockType type) {
    // const float spacing = 16.f / 256.f;

    switch (type) {
    case BlockType::Grass:
        m_textureFormat.top = 0;
        m_textureFormat.side = 3;
        m_textureFormat.bottom = 2;
        break;

    case BlockType::Dirt:
        m_textureFormat.top = 2;
        m_textureFormat.side = 2;
        m_textureFormat.bottom = 2;
        break;
    case BlockType::Stone:
        m_textureFormat.top = 1;
        m_textureFormat.side = 1;
        m_textureFormat.bottom = 1;
        break;
    case BlockType::Bedrock:
        m_textureFormat.top = 17;
        m_textureFormat.side = 17;
        m_textureFormat.bottom = 17;
        break;
    default:
        break;
    }

    return m_textureFormat;

    // switch (type) {
    // case BlockType::Grass:
    // m_textureFormat.top = TextureUVS{m_uvArray[0], m_uvArray[1], m_uvArray[2], m_uvArray[3], m_uvArray[4], m_uvArray[5], m_uvArray[6], m_uvArray[7]};

    // m_textureFormat.side = TextureUVS{m_uvArray[0 + (8 * 3)], m_uvArray[1 + (8 * 3)], m_uvArray[2 + (8 * 3)], m_uvArray[3 + (8 * 3)], m_uvArray[4 + (8 * 3)], m_uvArray[5 + (8 * 3)], m_uvArray[6 + (8 * 3)], m_uvArray[7 + (8 * 3)]};

    // m_textureFormat.bottom = TextureUVS{m_uvArray[0 + (8 * 2)], m_uvArray[1 + (8 * 2)], m_uvArray[2 + (8 * 2)], m_uvArray[3 + (8 * 2)], m_uvArray[4 + (8 * 2)], m_uvArray[5 + (8 * 2)], m_uvArray[6 + (8 * 2)], m_uvArray[7 + (8 * 2)]};
    // break;
    // case BlockType::Dirt:
    //     m_textureFormat.top = m_uvArray[2];
    //     m_textureFormat.side = m_uvArray[2];
    //     m_textureFormat.bottom = m_uvArray[2];
    //     break;
    // case BlockType::Stone:
    //     m_textureFormat.top = m_uvArray[1];
    //     m_textureFormat.side = m_uvArray[1];
    //     m_textureFormat.bottom = m_uvArray[1];
    //     break;
    // case BlockType::Bedrock:
    //     m_textureFormat.top = m_uvArray[17];
    //     m_textureFormat.side = m_uvArray[17];
    //     m_textureFormat.bottom = m_uvArray[17];
    // default:
    //     break;
    // }
    // return m_textureFormat;
}
