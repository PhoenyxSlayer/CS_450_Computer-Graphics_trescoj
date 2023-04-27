#version 430 core
// Change to 410 for macOS

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec3 normal;

out vec4 vertexColor;
out vec4 interPos;
out vec3 interNormal;

uniform mat3 normalMat;
uniform mat4 modelMat, viewMat, projMat;

void main()
{		
	// Get position of vertex (object space)
	vec4 objPos = vec4(position, 1.0);
	vec4 viewPos = viewMat*modelMat*objPos;
	interPos = viewPos;

	// For now, just pass along vertex position (no transformations)
	gl_Position = projMat*viewPos;

	normalMat = transpose(inverse(mat3(viewMat*modelMat)));
	interNormal = normalMat*normal;

	// Output per-vertex color
	vertexColor = color;
}
