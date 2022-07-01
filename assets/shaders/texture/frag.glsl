#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D myTexture;

void main() {
    fragColor = texture(myTexture, uv);
    if (fragColor.a == 0) {
        discard;
    }
}
