#version 330

layout(location = 4) in vec3 vertex_position;

out vec3 vertexpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vertexpos = vertex_position;

    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    gl_PointSize = 5.0;
};
