#version 430 core
// Change to 410 for macOS

layout(location=0) out vec4 out_color;
 
in vec4 vertexColor; // Now interpolated across face

void main()
{	
	// Just output interpolated color
	out_color = vertexColor;
}
