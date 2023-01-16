#ifndef GL_SETUP_H
#define GL_SETUP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
using namespace std;

GLFWwindow* setupGLFW(int major, int minor, int windowWidth, int windowHeight, bool debugging);
void cleanupGLFW(GLFWwindow* window);
void setupGLEW(GLFWwindow* window);
void checkOpenGLVersion();
void checkAndSetupOpenGLDebugging();

#endif
