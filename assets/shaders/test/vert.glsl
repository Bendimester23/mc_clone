#version 330 core

//int
//x    y    z    u    v    light
//0000 0000 0000 0000 0000 0000  0000 0000

layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 projectionMat;

void main() {
    gl_Position = projectionMat * vec4(aPos, 1.0);
    pos = aPos;
}