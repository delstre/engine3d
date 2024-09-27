#version 330

in vec2 uv;
in vec3 normal;
in vec3 vertexpos;

out vec4 frag_color;

uniform sampler2D my_texture;
uniform int side;

void main() {
    vec4 texColor = texture(my_texture, uv);

    vec3 posAbs  = abs(vertexpos);
    vec3 color = vec3(1.0);
    vec3 add = vec3(0.0);

    vec3 s = step(posAbs.yzx, posAbs) * step(posAbs.zxy, posAbs);
    if ((side == 1 || side == 2) && s.x > 0) {
        add = vec3(0.8);
    }
    if ((side == 3 || side == 4) && s.z >= 1) {
        add = vec3(0.8);
    }
    if ((side == 5 || side == 6) && s.y >= 1) {
        add = vec3(0.8);
    }

    color += add;

    frag_color = texColor * vec4(color, 1.0); // Красный цвет
}
