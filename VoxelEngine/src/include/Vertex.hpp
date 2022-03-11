#pragma once
#include <cstdint>

struct UV {
    float u, v;
};

struct TextureUVS {
    UV uvs[4];
};

struct Vertex {
    // Bits 00-19 Position
    // Bits 20-22 Normal
    // Bits 23-24 vertex
    // Bits 25-31 texturebufferID/index
    uint32_t data;
};