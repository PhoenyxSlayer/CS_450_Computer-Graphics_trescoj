#version 430 core

layout(location=0) in vec3 position;

void main() {
    vec3 pos = position;
    pos = pos + vec3(0.2, -0.1, 0.0);

    gl_Position = vec4(pos, 1.0);
}
