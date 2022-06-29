#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 projectionMat;

void main() {
    gl_Position = projectionMat * vec4(aPos, 1.0);
    pos = aPos;
}