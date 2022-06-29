#version 330 core

in vec3 pos;

out vec4 fragColor;

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

void main() {
    if (abs(pos.x) > 0.95f && abs(pos.y) > 0.95f) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(0.9, 0.5, 0.34, 1.0);
    }
    //fragColor = vec4(vec3(gl_FragCoord.xy/vec2(1280, 720), 1.0), 1.0);
}
