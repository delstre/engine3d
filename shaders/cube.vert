#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec3 color;
out vec2 uv;
out vec3 vertexpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    uv = vertex_uv;

    vertexpos = vertex_position;

    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
};
