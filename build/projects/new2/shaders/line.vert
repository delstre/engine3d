#version 330

layout(location = 0) in vec3 vertex_position;

uniform mat4 mvp;
uniform mat4 model;

out vec2 uv;

void main() {
    gl_Position = mvp * vec4(vertex_position, 1.0);
};
