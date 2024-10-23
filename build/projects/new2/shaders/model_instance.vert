#version 430 core

layout(std430, binding = 0) buffer ModelPositions {
    mat4 models[];
} buffer_data;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 mvp;

out vec2 uv;
out vec3 position;
out vec3 normals;

flat out int instanceID;

mat4 translate(vec3 translation) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        translation.x, translation.y, translation.z, 1.0
    );
}

void main() {
    uv = vertex_uv;
    position = vertex_position;
    normals = vertex_normals;

    gl_Position = mvp * buffer_data.models[gl_InstanceID] * vec4(vertex_position, 1.0);
    instanceID = gl_InstanceID;
};
