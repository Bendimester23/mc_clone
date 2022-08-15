#version 330 core

//x     y     z     u     v     light side
//00000 00000 00000 00000 00000 0000  000

//side:
//0 - top
//1 - s
//2 - w
//3 - e
//4 - n
//5 - bottom

layout (location = 0) in uint data;

uniform mat4 projectionMatrix;
uniform vec3 chunkPos;

vec3 pos;
out vec2 uv;
out float light;

float getSideLightModifier() {
    return float(data & uint(7)) * 0.02;
}

float max(float a, float b) {
    if (a > b) return a;
    return b;
}

void unpackData() {
    uint mask = uint(31);
    pos.x = data >> 27;
    pos.y = (data >> 22) & mask;
    pos.z = (data >> 17) & mask;

    uv.x = float((data >> 12) & mask)/16.0;
    uv.y = float((data >> 7) & mask)/16.0;

    light = max(float(((data >> 3) & uint(15)) / 15.0) - getSideLightModifier(), 0.08);
}

void main() {
    unpackData();
    gl_Position = projectionMatrix * vec4(chunkPos*16 + pos, 1.0);
}
