#include "Utility.hpp"

void aiMatToGLM4(aiMatrix4x4 &a, glm::mat4 &m) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m[j][i] = a[i][j];
        }
    }
}

void printTab(int cnt) {
    for(int i = 0; i < cnt; i++) {
        cout << "\t";
    }
}

void printNodeInfo(aiNode *node, glm::mat4 &nodeT, glm::mat4 &parentMat, glm::mat4 &currentMat, int level) {
    printTab(level);
    cout << "NAME: " << node->mName.C_Str() << endl;
    printTab(level);
    cout << "NUM MESHES: " << node->mNumMeshes << endl;
    printTab(level);
    cout << "NUM CHILDREN: " << node->mNumChildren << endl;
    printTab(level);
    cout << "Parent Model Matrix:" << glm::to_string(parentMat) << endl;
    printTab(level);
    cout << "Node Transforms:" << glm::to_string(nodeT) << endl;
    printTab(level);
    cout << "Current Model Matrix:" << glm::to_string(currentMat) << endl;
    cout << endl;
}
