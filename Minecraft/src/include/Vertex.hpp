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
    // Bits 19-22 Normal
    // Bits 22-24 vertex
    // Bits 24-31 texturebufferID/index
    uint32_t data;
};