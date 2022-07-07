#version 330 core

in vec2 uv;
flat in uint light;

uniform sampler2D atlas;

out vec4 fragColor;

void main() {
    fragColor = texture(atlas, vec2(uv.x/16, uv.y/16));
    if (fragColor.a == 0) {
        discard;
    }
//    fragColor *= max(light / 15.0f, 0.2f);
}
