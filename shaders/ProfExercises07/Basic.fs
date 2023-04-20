#version 430 core

layout(location=0) out vec4 out_color;

struct PointLight {
    vec4 pos;
    vec4 color;
};

uniform PointLight light;

in vec4 interPos;
in vec3 interNormal;

void main() {

    vec4 kd = vec4(0,0,1,1);

    vec3 L = vec3(light.pos - interPos);
    float dist = length(L);
    L = normalize(L);

    float at = 1.0/(dist*dist + 1.0);

    vec3 N = normalize(interNormal);

    float diffFactor = max(0.0, dot(N, L));

    vec4 diffColor = kd*light.color*diffFactor;


    //out_color = vec4(0.0, 0.0, 1.0, 1.0);
    //out_color = vec4(at, at, at, 1.0);
    //out_color = vec4(N, 1.0);
    out_color = diffColor;
}
