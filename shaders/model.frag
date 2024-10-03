#version 430

in vec2 uv;
in vec3 color;
in vec3 vertexpos; 

out vec4 frag_color;

uniform sampler2D my_texture;

void main() {
    //int side = 1;
    //vec3 posAbs  = abs(vertexpos);
    //vec3 add = vec3(0.0);
    //vec3 col = vec3(0.5);

    //vec3 s = step(posAbs.yzx, posAbs) * step(posAbs.zxy, posAbs);
    //if ((side == 1 || side == 2) && s.x > 0) {
        //add = vec3(0.5);
    //}
    //if ((side == 3 || side == 4) && s.z >= 1) {
        //add = vec3(0.5);
    //}
    //if ((side == 5 || side == 6) && s.y >= 1) {
        //add = vec3(0.5);
    //}

    //col += add;

    vec4 texColor = texture(my_texture, uv);
    if (texColor == vec4(0.0, 0.0, 0.0, 1.0)) {
        frag_color = vec4(color, 1.0);
    } else {
        frag_color = texColor * vec4(color, 1.0); // Красный цвет
    }
}
