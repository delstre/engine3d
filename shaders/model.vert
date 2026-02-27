#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 mvp;
uniform mat4 model;

out vec2 uv;
out vec3 color;
out vec3 position;
out vec3 normals;
flat out int triangleID;
flat out int vertexID;

void main() {
    uv = vertex_uv;
    position = vertex_position;
    normals = vertex_normals;
    triangleID = gl_VertexID / 3;
    vertexID = gl_VertexID;
    gl_Position = mvp * model * vec4(vertex_position, 1.0);
};
