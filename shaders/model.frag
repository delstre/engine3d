#version 330

in vec2 uv;

out vec4 frag_color;

uniform sampler2D my_texture;

void main() {
    frag_color = texture(my_texture, uv) * vec4(0.5, 0.5, 0.5, 1.0); // Красный цвет
}
