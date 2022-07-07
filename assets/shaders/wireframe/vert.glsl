#version 330 core

//x    y    z    u    v    light
//0000 0000 0000 0000 0000 0000  0000 0000
layout (location = 0) in uint data;

uniform mat4 projectionMat;
uniform vec3 chunkPos;

uint mask = uint(31);

vec3 unpackData() {
    uint x = data >> 27;
    uint y = (data >> 22) & mask;
    uint z = (data >> 17) & mask;

    return vec3(x, y, z);
}

void main() {
    gl_Position = projectionMat * vec4(unpackData() + (chunkPos * 16), 1.0f);
}
