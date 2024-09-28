#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 mvp;
uniform mat4 model;

out vec2 uv;

void main() {
    uv = vertex_uv;
    gl_Position = mvp * model * vec4(vertex_position, 1.0);
};
