#include "TextureAtlas.hpp"

#include "stb_image.h"

#include <GLAD/glad.h>
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
}
void TextureAtlas::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureAtlas::generateUVS() {
    const int textureSize = 16;
    m_uvArray = new TextureUVS[(m_width / textureSize) * (m_height / textureSize)];
    const float spacing = 16.f / 256.f;

    for (int y = 16, index = 0; y > 0; y--) {
        for (int x = 0; x < 16; x++, index++) {
            m_uvArray[index].uvs[0] = {(float)x * spacing, (float)y * spacing};
            m_uvArray[index].uvs[1] = {(float)x * spacing, (((float)y * spacing) - spacing)};
            m_uvArray[index].uvs[2] = {((float)x * spacing) + spacing, (((float)y * spacing) - spacing)};
            m_uvArray[index].uvs[3] = {((float)x * spacing) + spacing, (float)y * spacing};
        }
    }
}

TextureFormat TextureAtlas::getUVS(BlockType type) {
    const float spacing = 16.f / 256.f;

    // clang-format off
    switch (type) {
    case BlockType::Grass:
        m_textureFormat.top = m_uvArray[0];
        m_textureFormat.side = m_uvArray[3];
        m_textureFormat.bottom = m_uvArray[2];
        break;
    case BlockType::Dirt:
        m_textureFormat.top = m_uvArray[2];
        m_textureFormat.side = m_uvArray[2];
        m_textureFormat.bottom = m_uvArray[2];
        break;
    case BlockType::Stone:
        m_textureFormat.top = m_uvArray[1];
        m_textureFormat.side = m_uvArray[1];
        m_textureFormat.bottom = m_uvArray[1];
        break;
    case BlockType::Bedrock:
        m_textureFormat.top = m_uvArray[17];
        m_textureFormat.side = m_uvArray[17];
        m_textureFormat.bottom = m_uvArray[17];
    default:
        break;
    }
    //clang-format on
    return m_textureFormat;
}