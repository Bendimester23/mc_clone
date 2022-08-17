#version 330 core

in vec2 uv;
in float light;

out vec4 fragColor;

uniform sampler2D atlas;

uniform bool wireframe;

void main() {
    if (wireframe) {
        fragColor = vec4(0.27, 0.27, 0.98, 1);
        return;
    }
    fragColor = texture(atlas, uv) * (light*light*light);
    if (fragColor.a == 0) {
        discard;
    }
}
