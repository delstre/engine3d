#version 430

in vec2 uv;
in vec3 position; 
in vec3 normals;

out vec4 frag_color;

uniform sampler2D my_texture;
uniform vec3 color;

//uniform vec3 lightPos;
//uniform vec3 viewDir;
uniform vec3 viewPos;

//void main() {
    ////int side = 1;
    ////vec3 posAbs  = abs(vertexpos);
    ////vec3 add = vec3(0.0);
    ////vec3 col = vec3(0.5);

    ////vec3 s = step(posAbs.yzx, posAbs) * step(posAbs.zxy, posAbs);
    ////if ((side == 1 || side == 2) && s.x > 0) {
        ////add = vec3(0.5);
    ////}
    ////if ((side == 3 || side == 4) && s.z >= 1) {
        ////add = vec3(0.5);
    ////}
    ////if ((side == 5 || side == 6) && s.y >= 1) {
        ////add = vec3(0.5);
    ////}

    ////col += add;

    //vec3 col = vec3(0.6, 0.6, 0.6);
    //vec4 texColor = texture(my_texture, uv);
    //if (texColor == vec4(0.0, 0.0, 0.0, 1.0)) {
        //frag_color = vec4(col, 1.0);
    //} else {
        //frag_color = texColor * vec4(color, 1.0); // Красный цвет
    //}
//}

vec3 light() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // Диффузное освещение
    vec3 norm = normalize(normals); // Нормализация нормали
    vec3 lightDir = normalize(viewPos - position); // Вектор направления к источнику света
    float diff = max(dot(norm, lightDir), 0.0); // Вычисление диффузного компонента

    // Зеркальное освещение
    vec3 viewDir = normalize(viewPos - position); // Вектор направления к камере
    vec3 reflectDir = reflect(-lightDir, norm); // Отражённый вектор
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Вычисление зеркального компонента

    // Итоговый цвет
    vec3 ambient = 0.1 * lightColor; // Фоновое освещение
    vec3 diffuse = diff * lightColor; // Диффузный цвет
    vec3 specular = spec * lightColor; // Зеркальный цвет

    return vec3(ambient + diffuse + specular);
}

void main() {
    vec3 object_color = color; // vec3(0.6, 0.6, 0.6);

    vec4 texColor = texture(my_texture, uv);
    if (texColor == vec4(0.0, 0.0, 0.0, 1.0)) {
        frag_color = vec4(light() * object_color, 1.0); // Смешивание всех компонентов
    } else {
        frag_color = texColor * vec4(light() * object_color, 1.0); // Смешивание всех компонентов
    }
}

