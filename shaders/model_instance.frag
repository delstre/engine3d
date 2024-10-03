#version 430 core

layout(std430, binding = 1) buffer ModelTextures {
    uint instanceTextureIndices[];
};

layout(std430, binding = 2) buffer ModelColors {
    vec3 colors[];
};

in vec2 uv;
in vec3 color;
in vec3 vertexpos; 

flat in int instanceID;

out vec4 frag_color;

uniform int active_id;
uniform int call_id;

uniform sampler2D textures[3];

void main() {
    int side = 1;
    vec3 posAbs  = abs(vertexpos);
    vec3 add = vec3(0.0);
    vec3 col = colors[instanceID]; 
    //vec3 col = vec3(1.0);

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

    ////color += add;

    
    if (instanceID == active_id && call_id != active_id) {
        col = vec3(1.0);
    }

    if (instanceID == call_id) {
        col = vec3(1.0, 0.0, 0.0);
    }

    //vec4 texColor = texture(sampler2DArray textureSampler, vec3(0.0, 0.0, textureIndex));
    uint textureIndex = instanceTextureIndices[instanceID];

    if (textureIndex == 0) {
        frag_color = vec4(col, 1.0); // Красный цвет
    } else {
        vec4 texColor = texture(textures[textureIndex], uv);
        frag_color = texColor * vec4(col, 1.0); // Красный цвет
    }
}
