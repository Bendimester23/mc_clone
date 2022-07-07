#version 330 core

//x    y    z    u    v    light
//0000 0000 0000 0000 0000 0000  0000 0000
layout (location = 0) in int data;

uniform mat4 projectionMat;
uniform vec3 chunkPos;

vec3 unpackData() {
    int x = data >> 28;
    int y = (data >> 24) & 15;
    int z = (data >> 20) & 15;

    return vec3(x, y, z);
}

void main() {
    gl_Position = projectionMat * vec4(unpackData() + (chunkPos * 16), 1.0f);
}
