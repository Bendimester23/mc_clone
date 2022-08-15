#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 TexCoord;

uniform mat4 projectionMatrix;

out vec2 uv;

void main() {
    gl_Position = projectionMatrix * vec4(pos, 1.0);
    uv = TexCoord;
}
