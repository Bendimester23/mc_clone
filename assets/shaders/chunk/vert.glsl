#version 330 core

//x     y     z     u    v    light
//00000 00000 00000 0000 0000 0000  00000
layout (location = 0) in uint data;

uniform mat4 projectionMat;
uniform vec3 chunkPos;

out vec2 uv;
flat out uint light;

uint maskP = uint(31);
uint maskU = uint(15);

vec3 unpackData() {
    uint x = data >> 27;
    uint y = (data >> 22) & maskP;
    uint z = (data >> 17) & maskP;

    uint u = (data >> 13) & maskU;
    uint v = (data >> 9) & maskU;
    uv = vec2(u, v);

    light = (data >> 5) & maskU;
    return vec3(x, y, z);
}

void main() {
    gl_Position = projectionMat * vec4(unpackData() + (chunkPos * 16), 1.0f);
}
