#pragma once

struct UV {
    float u, v;
};

struct TextureUVS {
    UV uvs[4];
};

struct Vertex {
    struct Position {
        float x, y, z;
    } position;
    struct Normal {
        float x, y, z;
    } normal;
    UV uv;
};