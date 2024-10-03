#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_color;

uniform mat4 mvp;
uniform mat4 model;

out vec2 uv;
out vec3 color;
out vec3 vertexpos;

void main() {
    uv = vertex_uv;
    color = vertex_color;
    vertexpos = vertex_position;
    gl_Position = mvp * model * vec4(vertex_position, 1.0);
};
