#version 430 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec4 interPos;
out vec3 interNormal;

void main() {
    vec4 pos = vec4(position, 1.0);
    vec4 viewPos = viewMat*modelMat*pos;
    interPos = viewPos;
    
    gl_Position = projMat*viewPos; 

    mat3 normMat = transpose(inverse(mat3(viewMat*modelMat)));
    interNormal = normMat*normal;   
}
