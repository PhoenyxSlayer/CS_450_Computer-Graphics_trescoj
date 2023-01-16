#ifndef MESH_GL_DATA_H
#define MESH_GL_DATA_H

#include <iostream>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "MeshData.hpp"
using namespace std;

// Struct for holding OpenGL mesh
struct MeshGL {
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLuint VAO = 0;
	int indexCnt = 0;
};

void createMeshGL(Mesh &m, MeshGL &mgl);
void drawMesh(MeshGL &mgl);
void cleanupMesh(MeshGL &mgl);

#endif
