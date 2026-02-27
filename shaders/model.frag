#version 430

in vec2 uv;
in vec3 color;
in vec3 position; 
in vec3 normals;
flat in int triangleID;
flat in int vertexID;

out vec4 frag_color;

uniform sampler2D my_texture;

uniform vec3 viewPos;
uniform vec3 highlightColor;
uniform vec3 highlightColor_Vertex;
uniform int selectTriangle = -1;
uniform int selectVertex = -1;

vec3 light() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(viewPos - position);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;

    return vec3(ambient + diffuse + specular);
}

void main() {
    vec3 object_color = highlightColor;

    if (selectVertex >= 0 && vertexID == selectVertex) {
        frag_color = vec4(highlightColor_Vertex, 0.7);
        return;
    }

    if (selectTriangle >= 0 && triangleID == selectTriangle) {
        frag_color = vec4(highlightColor_Vertex, 0.7);
        return;
    }

    vec4 texColor = texture(my_texture, uv);
    if (texColor == vec4(0.0, 0.0, 0.0, 1.0)) {
        frag_color = vec4(light() * object_color, 1.0);
    } else {
        frag_color = texColor * vec4(light() * object_color, 1.0);
    }
}

