#version 450 core

layout (location = 0) in uint aData;

uniform samplerBuffer uTexUV;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 fTexCoord;
flat out uint fFace;

uint unpackTextureIndex(uint data);
uvec3 unpackPosition(uint data);
uint unpackFace(uint data);
vec2 setupTexCoord();

void main() {
    uvec3 pos = unpackPosition(aData);
    gl_Position = uProjection * uView * uModel * uvec4(pos, 1.0);
    fTexCoord = setupTexCoord();
    fFace = unpackFace(aData);
}

uvec3 unpackPosition(uint data) {
    return uvec3((data & 0x1F), ((data & 0x3FE0) >> 5), ((data & 0x7C000) >> 14));
}

uint unpackFace(uint data) {
    return ((data & 0x380000) >> 19);
}

uint unpackTextureIndex(uint data) {
    return ((data & 0xFF000000) >> 24);
}

vec2 setupTexCoord() {
    uint vertex = ((aData & 0xC00000) >> 22);
    int texIndex = int((unpackTextureIndex(aData) * 8) + (vertex*2));

    return vec2(texelFetch(uTexUV, texIndex + 0).r, texelFetch(uTexUV, texIndex + 1).r);
}