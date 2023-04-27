#include "MeshGLData.hpp"

// Create OpenGL mesh (VAO) from mesh data
void createMeshGL(Mesh &m, MeshGL &mgl) {
	// Create Vertex Buffer Object (VBO)
	glGenBuffers(1, &(mgl.VBO));
	glBindBuffer(GL_ARRAY_BUFFER, mgl.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m.vertices.size(), m.vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Create Vertex Array Object (VAO)
	glGenVertexArrays(1, &(mgl.VAO));

	// Enable VAO
	glBindVertexArray(mgl.VAO);

	// Enable the first two vertex attribute arrays
	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);
	
	// Bind the VBO and set up data mappings so that VAO knows how to read it
	// 0 = pos (3 elements)
	// 1 = color (4 elements)
	glBindBuffer(GL_ARRAY_BUFFER, mgl.VBO);	

	// Attribute, # of components, type, normalized?, stride, array buffer offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Create Element Buffer Object (EBO)
	glGenBuffers(1, &(mgl.EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mgl.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		m.indices.size() * sizeof(GLuint),
		m.indices.data(),
		GL_STATIC_DRAW);

	// Set index count
	mgl.indexCnt = (int)m.indices.size();

	// Unbind vertex array for now
	glBindVertexArray(0);
}

// Draw OpenGL mesh
void drawMesh(MeshGL &mgl) {
	glBindVertexArray(mgl.VAO);
	glDrawElements(GL_TRIANGLES, mgl.indexCnt, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);		
}

// Cleanup OpenGL mesh
void cleanupMesh(MeshGL &mgl) {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(mgl.VBO));
	mgl.VBO = 0;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(mgl.EBO));
	mgl.EBO = 0;

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &(mgl.VAO));
	mgl.VAO = 0;

	mgl.indexCnt = 0;
}
