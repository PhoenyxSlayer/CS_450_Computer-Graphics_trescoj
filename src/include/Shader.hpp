#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
using namespace std;

string readFileToString(string filename);
void printShaderCode(string &vertexCode, string &fragCode);
GLuint createAndCompileShader(const char *shaderCode, GLenum shaderType);
GLuint createAndLinkShaderProgram(std::vector<GLuint> allShaderIDs);
GLuint initShaderProgramFromSource(string vertexShaderCode, string fragmentShaderCode);

#endif
