#version 330 core

in vec3 pos;

out vec4 fragColor;

bool nearCorner(float a) {
        return abs(a-round(a)) < 0.05f;
}

void main() {
    if ((nearCorner(pos.x) && nearCorner(pos.y)) || (nearCorner(pos.x) && nearCorner(pos.z)) || (nearCorner(pos.z) && nearCorner(pos.y))) {
        fragColor = vec4(vec3(gl_FragCoord.xy/vec2(1280, 720), 1.0), 1.0);
    } else {
        fragColor = vec4(0.9, 0.5, 0.34, 1.0);
    }
}
