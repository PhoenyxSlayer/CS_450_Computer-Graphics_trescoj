#version 430 core
// Change to 410 for macOS

layout(location=0) out vec4 out_color;
 
in vec4 vertexColor; // Now interpolated across face
in vec4 interPos;
in vec3 interNormal;
uniform vec4 lightPos;

void main()
{	
	vec3 N = normalize(interNormal);
	vec3 L = vec3(lightPos.pos - interPos);
	L = normalize(L);

	float diffFactor = max(0.0, dot(N, L));
	vec3 diffColor = vertexColor*diffFactor;
	// Just output interpolated color
	out_color = diffColor;
}
