#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 fTexCoord;
out vec3 fNormal;

void main() {
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
    fTexCoord = aTexCoord;
    fNormal = aNormal;
}