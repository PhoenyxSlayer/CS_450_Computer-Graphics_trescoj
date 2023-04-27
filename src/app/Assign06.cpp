#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "MeshData.hpp"
#include "MeshGLData.hpp"
#include "GLSetup.hpp"
#include "Shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utility.hpp"

using namespace std;

float rotAngle = 0.0f;

glm::vec3 cameraPos(0.0f, 0.0f, 1.0f), cameraLook(0.0f, 0.0f, 0.0f);
glm::vec2 mousePos;

// Create very simple mesh: a quad (4 vertices, 6 indices, 2 triangles)
void createSimpleQuad(Mesh& m) {
	// Clear out vertices and elements
	m.vertices.clear();
	m.indices.clear();

	// Create four corners
	Vertex upperLeft, upperRight;
	Vertex lowerLeft, lowerRight;

	// Set positions of vertices
	// Note: glm::vec3(x, y, z)
	upperLeft.position = glm::vec3(-0.5, 0.5, 0.0);
	upperRight.position = glm::vec3(0.5, 0.5, 0.0);
	lowerLeft.position = glm::vec3(-0.5, -0.5, 0.0);
	lowerRight.position = glm::vec3(0.5, -0.5, 0.0);

	// Set vertex colors (red, green, blue, white)
	// Note: glm::vec4(red, green, blue, alpha)
	upperLeft.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	upperRight.color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	lowerLeft.color = glm::vec4(0.0, 0.0, 1.0, 1.0);
	lowerRight.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Add to mesh's list of vertices
	m.vertices.push_back(upperLeft);
	m.vertices.push_back(upperRight);
	m.vertices.push_back(lowerLeft);
	m.vertices.push_back(lowerRight);

	// Add indices for two triangles
	m.indices.push_back(0);
	m.indices.push_back(3);
	m.indices.push_back(1);

	m.indices.push_back(0);
	m.indices.push_back(2);
	m.indices.push_back(3);
}

void createSimplePentagon(Mesh& m) {
	// Clear out vertices and elements
	m.vertices.clear();
	m.indices.clear();

	// Create four corners
	Vertex upperLeft, upperRight;
	Vertex lowerLeft, lowerRight;
	Vertex vert5;

	// Set positions of vertices
	// Note: glm::vec3(x, y, z)
	upperLeft.position = glm::vec3(-0.5, 0.5, 0.0);
	upperRight.position = glm::vec3(0.5, 0.5, 0.0);
	lowerLeft.position = glm::vec3(-0.5, -0.5, 0.0);
	lowerRight.position = glm::vec3(0.5, -0.5, 0.0);
	vert5.position = glm::vec3(-0.999, 0.0, 0.5);

	// Set vertex colors (red, green, blue, white)
	// Note: glm::vec4(red, green, blue, alpha)
	upperLeft.color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	upperRight.color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	lowerLeft.color = glm::vec4(0.0, 0.0, 1.0, 1.0);
	lowerRight.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	vert5.color = glm::vec4(0.9, 0.45, 1.0, 1.0);

	// Add to mesh's list of vertices
	m.vertices.push_back(upperLeft);
	m.vertices.push_back(upperRight);
	m.vertices.push_back(lowerLeft);
	m.vertices.push_back(lowerRight);
	m.vertices.push_back(vert5);

	// Add indices for two triangles
	m.indices.push_back(0);
	m.indices.push_back(3);
	m.indices.push_back(1);

	m.indices.push_back(0);
	m.indices.push_back(2);
	m.indices.push_back(3);

	m.indices.push_back(0);
	m.indices.push_back(2);
	m.indices.push_back(4);
}

void extractMeshData(aiMesh *mesh, Mesh &m);

glm::mat4 makeRotateZ(glm::vec3);

void renderScene(vector<MeshGL> &allMeshes, aiNode *node, glm::mat4 parentMat, GLint modelMatLoc, int level);

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

glm::mat4 makeLocalRotate(glm::vec3, glm::vec3, float);

static void mouse_position_callback(GLFWwindow *window, double, double);

// Main 
int main(int argc, char** argv) {
	Assimp::Importer importer;
	string modelPath =  "sampleModels/sphere.obj";
	vector<MeshGL> objMeshes;
	double mx, my;

	if(argc >= 2) modelPath = argv[1];

	const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) cerr<<"Error: "<<importer.GetErrorString()<<endl;
	
	// Are we in debugging mode?
	bool DEBUG_MODE = true;

	// GLFW setup
	// Switch to 4.1 if necessary for macOS
	GLFWwindow* window = setupGLFW(4, 3, 800, 800, DEBUG_MODE);
	glfwSetKeyCallback(window, key_callback);

	// GLEW setup
	setupGLEW(window);

	// Check OpenGL version
	checkOpenGLVersion();

	// Set up debugging (if requested)
	if (DEBUG_MODE) checkAndSetupOpenGLDebugging();

	// Set the background color to a shade of blue
	glClearColor(0.3f, 0.0f, 0.3f, 1.0f);

	// Create and load shader
	GLuint programID = 0; 
	try {
		// Load vertex shader code and fragment shader code
		string vertexCode = readFileToString("shaders/Assign06/Basic.vs");
		string fragCode = readFileToString("shaders/Assign06/Basic.fs");

		// Print out shader code, just to check
		if (DEBUG_MODE) printShaderCode(vertexCode, fragCode);

		// Create shader program from code
		programID = initShaderProgramFromSource(vertexCode, fragCode);
	}
	catch (exception e) {
		// Close program
		cleanupGLFW(window);
		exit(EXIT_FAILURE);
	}

	// Create simple quad
	/*Mesh m;
	createSimplePentagon(m);

	// Create OpenGL mesh (VAO) from data
	MeshGL mgl;
	createMeshGL(m, mgl);*/
	for(int i = 0; (unsigned int)i < scene->mNumMeshes; i++){
		Mesh m;
		MeshGL mgl;
		extractMeshData(scene->mMeshes[i], m);
		createMeshGL(m, mgl);
		objMeshes.push_back(mgl);
	}
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	glfwGetCursorPos(window, &mx, &my);
	mousePos = glm::vec2(mx, my);
	glfwSetCursorPosCallback(window, mouse_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLint location = glGetUniformLocation(programID, "modelMat");
	GLint viewLoc = glGetUniformLocation(programID, "viewMat");
	GLint projLoc = glGetUniformLocation(programID, "projMat");

	while (!glfwWindowShouldClose(window)) {
		// Set viewport size
		int fwidth, fheight;
		glfwGetFramebufferSize(window, &fwidth, &fheight);
		glViewport(0, 0, fwidth, fheight);

		// Clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader program
		glUseProgram(programID);

		// Draw object]
		//drawMesh(mgl);
		/*for(int i = 0; i < objMeshes.size(); i++){
			drawMesh(objMeshes[i]);
		}*/

		glm::mat4 view = glm::lookAt(cameraPos, cameraLook, glm::vec3(0,1,0));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float ratio = 0;

		if((width || height) == 0){
			ratio = 1;
		}else{
			ratio = (float)(width/height);
		}

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), ratio, 0.01f, 50.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		renderScene(objMeshes, scene->mRootNode, glm::mat4(1.0), location, 0);

		// Swap buffers and poll for window events		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Sleep for 15 ms
		this_thread::sleep_for(chrono::milliseconds(15));
	}

	// Clean up mesh
	//cleanupMesh(mgl);
	for(int i = 0; i < objMeshes.size(); i++){
		cleanupMesh(objMeshes[i]);
	}
	objMeshes.clear();

	// Clean up shader programs
	glUseProgram(0);
	glDeleteProgram(programID);
	// Destroy window and stop GLFW
	cleanupGLFW(window);

	return 0;
}

void extractMeshData(aiMesh *mesh, Mesh &m){
	Vertex vert;
	int i = 0;

	m.indices.clear();
	m.vertices.clear();

	for(i = 0; (unsigned int)i < mesh->mNumVertices; i++){
		float t = float(i) / float(mesh->mNumVertices);

		vert.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vert.color = glm::vec4(std::sin(glm::radians(t * 360.0f)), std::sin(glm::radians(t * 360.0f + 120.0f)), std::sin(glm::radians(t * 360.0f + 240.0f)), 1.0);
		m.vertices.push_back(vert);
	}

	for(i = 0; (unsigned int)i < mesh->mNumFaces; i++){
		for(int j = 0; (unsigned int)j < mesh->mFaces[i].mNumIndices; j++){
			m.indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	glm::vec3 direction;
	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		if(key == GLFW_KEY_ESCAPE){
			glfwSetWindowShouldClose(window, true);
		}else if(key == GLFW_KEY_J){
			rotAngle += 1.0f;
			cout<<rotAngle<<endl;
		}else if(key == GLFW_KEY_K){
			rotAngle -= 1.0f;
			cout<<rotAngle<<endl;
		}else if(key == GLFW_KEY_W){
			direction = cameraLook - cameraPos;
			direction = normalize(direction);
			cameraPos += direction * 0.1f;
			cameraLook += direction * 0.1f;			
		}else if(key == GLFW_KEY_S){			
			direction = cameraLook - cameraPos;
			direction = normalize(direction);
			cameraPos -= direction * 0.1f;
			cameraLook -= direction * 0.1f;
		}else if(key == GLFW_KEY_A){
			direction = cross((cameraLook-cameraPos), glm::vec3(0,1,0));
			direction = normalize(direction);
			cameraPos -= direction * 0.1f;
			cameraLook -= direction * 0.1f;
		}else if(key == GLFW_KEY_D){
			direction = cross((cameraLook-cameraPos), glm::vec3(0,1,0));
			direction = normalize(direction);
			cameraPos += direction * 0.1f;
			cameraLook += direction * 0.1f;
		}
	}
}

static void mouse_position_callback(GLFWwindow *window, double xpos, double ypos){
	glm::vec2 relMouse = glm::vec2(xpos, ypos) - mousePos;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	if(width > 0 && height > 0){
		relMouse.x /= -(float)width;
		relMouse.y /= (float)height;

		glm::mat4 xPosition = makeLocalRotate(cameraPos, glm::vec3(0,1,0), 30.0f*relMouse.x);
		glm::mat4 yPosition = makeLocalRotate(cameraPos, glm::cross(glm::vec3(0,1,0), (cameraLook-cameraPos)), 30.0f*relMouse.y);
		glm::vec4 lookAtV = glm::vec4(cameraLook, 1);
		lookAtV = xPosition*yPosition*lookAtV;
		cameraLook = glm::vec3(lookAtV);
		mousePos = glm::vec2(xpos, ypos);
	}
}

glm::mat4 makeRotateZ(glm::vec3 offset){
	float angle = glm::radians(rotAngle);

	glm::mat4 negTranslation = glm::translate(glm::mat4(1.0f), -offset);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), offset);
	glm::mat4 transform = translation * rotation * negTranslation;

	return transform;
}

void renderScene(vector<MeshGL> &allMeshes, aiNode *node, glm::mat4 parentMat, GLint modelMatLoc, int level){
	aiMatrix4x4 transformation = node->mTransformation;
	glm::mat4 nodeT;
	aiMatToGLM4(transformation, nodeT);

	glm::mat4 modelMat = parentMat*nodeT;
	glm::vec3 nodeLoc = glm::vec3(modelMat[3]);
	glm::mat4 R = makeRotateZ(nodeLoc);
	glm::mat4 tmpModel = R*modelMat;

	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(tmpModel));

	for(int i = 0; (unsigned int)i < node->mNumMeshes; i++){
		int index = node->mMeshes[i];
		drawMesh(allMeshes.at(index));
	}

	for(int i = 0; (unsigned int)i < node->mNumChildren; i++){
		aiNode* child = node->mChildren[i];
		renderScene(allMeshes, child, modelMat, modelMatLoc, level+1);		
	}
}

glm::mat4 makeLocalRotate(glm::vec3 offset, glm::vec3 axis, float angle){
	float radAngle = glm::radians(angle);

	glm::mat4 n = glm::translate(glm::mat4(1.0f), -offset);
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), radAngle, axis);
	glm::mat4 t = glm::translate(glm::mat4(1.0f), offset);
	glm::mat4 transform = t*r*n;

	return transform;
}