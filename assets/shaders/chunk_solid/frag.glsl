#version 330 core

in vec2 uv;
in float light;

out vec4 fragColor;

uniform sampler2D atlas;

void main() {
    fragColor = texture(atlas, uv) * (light*light*light);
    if (fragColor.a == 0) {
        discard;
    }
}
