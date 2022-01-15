#version 450 core

layout (location = 0) in uint aData;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 fTexCoord;
flat out uint fFace;

uvec3 unpackPosition(uint data);
uint unpackFace(uint data);

void main() {
    uvec3 pos = unpackPosition(aData);
    gl_Position = uProjection * uView * uModel * uvec4(pos, 1.0);
    fTexCoord = aTexCoord;
    fFace = unpackFace(aData);
}

uvec3 unpackPosition(uint data) {
    return uvec3((data & 0x1F), ((data & 0x3FE0) >> 5), ((data & 0x7C000) >> 14));
}

uint unpackFace(uint data) {
    return ((data & 0x380000) >> 19);
}