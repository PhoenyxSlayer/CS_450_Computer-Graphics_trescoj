#version 430 core

layout(location=0) in vec3 position;

uniform mat4 modelMat;

void main() {
    vec4 pos = vec4(position, 1.0);
    
    pos = modelMat*pos;
    
    gl_Position = pos;
}
