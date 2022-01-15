#pragma once
#include <cstdint>

struct UV {
    float u, v;
};

struct TextureUVS {
    UV uvs[4];
};

struct Vertex {
    uint32_t data;
    UV uv;
};