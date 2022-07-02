#version 330 core

//int
//x    y    z    u    v    lightColor light intensity
//0000 0000 0000 0000 0000 00000000   0000


layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uvIn;

out vec2 uv;

void main() {
    gl_Position = vec4(vec3(pos, 0.0f), 1.0f);
    uv = uvIn;
}
