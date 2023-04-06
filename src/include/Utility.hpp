#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <assimp/scene.h>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
using namespace std;

void aiMatToGLM4(aiMatrix4x4 &a, glm::mat4 &m);
void printTab(int cnt);
void printNodeInfo(aiNode *node, glm::mat4 &nodeT, glm::mat4 &parentMat, glm::mat4 &currentMat, int level);

#endif
