#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

glm::vec2 lastMousePos;

struct PointLight {
    glm::vec4 pos = glm::vec4(0,0,0,1);
    glm::vec4 color = glm::vec4(1,1,1,1);
};

vector<GLfloat> vertOnly = {
    -0.3f, -0.3f, 0.0f,
    +0.3f, -0.3f, 0.0f,
    -0.3f, +0.3f, 0.0f,

    -0.3f, +0.3f, 0.0f,
    +0.3f, -0.3f, 0.0f,
    +0.3f, +0.3f, 0.0f,
};

float squareSize = 0.7f;
vector<GLfloat> vertList = {
    -squareSize, +squareSize, 0.0f, 0.0f, 0.0f, 1.0f,
    +squareSize, +squareSize, 0.0f, 0.0f, 0.0f, 1.0f,
    -squareSize, -squareSize, 0.0f, 0.0f, 0.0f, 1.0f,
    +squareSize, -squareSize, 0.0f, 0.0f, 0.0f, 1.0f,
};

vector<GLuint> indexList = {
    0, 3, 1,
    2, 3, 0,
};

glm::mat4 modelMat(1.0);
float speed = 0.05f;

void printRM(string name, glm::mat4 &M) {
    cout << name << ": " << endl;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            cout << M[j][i] << ", ";
        }
        cout << endl;
    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
    else if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        cout << "Key: " << key << endl;
        glm::mat4 M(1.0);
        if(key == GLFW_KEY_A) {
            M = glm::translate(glm::vec3(-speed,0,0));            
        }
        else if(key == GLFW_KEY_D) {
            M = glm::translate(glm::vec3(+speed,0,0));
        }

        modelMat = M*modelMat;
    }    
}

static void error_callback(int error, const char* desc) {
    cout << "ERROR " << error << ": " << desc << endl;
}

static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
    glm::vec2 curMousePos = glm::vec2(xpos, ypos);
    cout << "MOUSE: " << glm::to_string(curMousePos) << endl;

    // MOUSE MAGIC

    lastMousePos = curMousePos;
}

int main(int argc, char **argv) {

    cout << "Begin Mathemagical adventure!" << endl;
    glm::vec3 A(1,4,0);
    glm::vec3 B = glm::vec3(2,3,2);

    cout << "A: " << glm::to_string(A) << endl;
    cout << "B: " << glm::to_string(B) << endl;
    cout << "A.x: " << A.x << endl;
    cout << "A.r: " << A.r << endl;

    glm::vec3 C = B - A;
    cout << "C: " << glm::to_string(C) << endl;

    glm::vec3 longA = A*5.0f;
    cout << "longA: " << glm::to_string(longA) << endl;

    float lenA = glm::length(A);
    float lenLongA = glm::length(longA);

    cout << "A lengths: " << lenA << " " << lenLongA << endl;

    glm::vec3 normA = glm::normalize(A);
    cout << "normA: " << glm::to_string(normA) << endl;
    cout << "length(normA): " << glm::length(normA) << endl;

    glm::vec4 test(1,0,0,1);
    cout << "length(test): " << glm::length(test) << endl;

    glm::vec3 normB = glm::normalize(B);
    float dotAB = glm::dot(normA, normB);
    cout << "dotAB: " << dotAB << endl;

    glm::vec3 N = glm::cross(A, B);
    N = glm::normalize(N);
    cout << "N: " << glm::to_string(N) << endl;

    glm::vec3 rN = glm::cross(B, A);
    rN = glm::normalize(rN);
    cout << "rN: " << glm::to_string(rN) << endl;








    cout << "Begin OpenGL adventure!" << endl;
    // Insert magic here

    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) {
        cerr << "GLFW FAILED!" << endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    int windowWidth = 640;
    int windowHeight = 480;
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight,
                                            "Amazing OpenGL Window", 
                                            NULL, NULL);

    if(!window) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = true;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        cout << "GLEW ERROR: ";
        cout << glewGetErrorString(err) << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    lastMousePos = glm::vec2(xpos, ypos);

    glfwSetCursorPosCallback(window, mouse_position_callback);


    int fwidth = 0;
    int fheight = 0;

    glClearColor(0.8f, 0.0f, 0.1f, 1.0f);

    GLuint progID = 0;

    try {
        string vertCode = readFileToString("./shaders/ProfExercises07/Basic.vs");
        string fragCode = readFileToString("./shaders/ProfExercises07/Basic.fs");

        cout << "VERTEX SHADER:" << endl;
        cout << vertCode << endl;

        cout << "FRAGMENT SHADER:" << endl;
        cout << fragCode << endl;

        GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

        char const *vertPtr = vertCode.c_str();
        char const *fragPtr = fragCode.c_str();
        glShaderSource(vertID, 1, &vertPtr, NULL);
        glShaderSource(fragID, 1, &fragPtr, NULL);

        glCompileShader(vertID);
        glCompileShader(fragID);

        progID = glCreateProgram();
        glAttachShader(progID, vertID);
        glAttachShader(progID, fragID);

        glLinkProgram(progID);

        glDeleteShader(vertID);
        glDeleteShader(fragID);        

    }
    catch(exception e) {
        cout << "ERROR: Shader failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    GLint modelMatLoc = glGetUniformLocation(progID,
                                            "modelMat");
    cout << "modelMatLoc: " << modelMatLoc << endl;

    GLint viewMatLoc = glGetUniformLocation(progID, "viewMat");
    GLint projMatLoc = glGetUniformLocation(progID, "projMat");

    GLint lightPosLoc = glGetUniformLocation(progID, "light.pos");
    GLint lightColorLoc = glGetUniformLocation(progID, "light.color");

    PointLight myLight;
    myLight.pos = glm::vec4(0,0,0.25,1);
    myLight.color = glm::vec4(1,1,1,1);

    // Setup VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    //glBufferData(   GL_ARRAY_BUFFER, 
    //                vertOnly.size()*sizeof(GLfloat), 
    //                vertOnly.data(), GL_STATIC_DRAW);

    glBufferData(   GL_ARRAY_BUFFER, 
                    vertList.size()*sizeof(GLfloat), 
                    vertList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Setup EBO
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(   GL_ELEMENT_ARRAY_BUFFER,
                    indexList.size()*sizeof(GLuint),
                    indexList.data(), GL_STATIC_DRAW);

    // Setup VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // normal
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 
                            6*sizeof(float), 0);                        

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 
                            6*sizeof(float), ((void*)(3*sizeof(float))));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);









    int vcnt = vertOnly.size()/3;

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &fwidth, &fheight);
        glViewport(0, 0, fwidth, fheight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(progID);

        glUniformMatrix4fv(modelMatLoc, 1, false, glm::value_ptr(modelMat));

        glm::mat4 viewMat = glm::lookAt(glm::vec3(1,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
        glUniformMatrix4fv(viewMatLoc, 1, false, glm::value_ptr(viewMat));

        float aspect = 1.0f;
        if(fwidth > 0 && fheight > 0) {
            aspect = ((float)fwidth)/((float)fheight);
        }
        glm::mat4 projMat = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 50.0f);
        glUniformMatrix4fv(projMatLoc, 1, false, glm::value_ptr(projMat));

        glm::vec4 viewLightPos = viewMat*myLight.pos;
        glUniform4fv(lightPosLoc, 1, glm::value_ptr(viewLightPos));
        glUniform4fv(lightColorLoc, 1, glm::value_ptr(myLight.color));

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, vcnt);
        glDrawElements(GL_TRIANGLES, indexList.size(),
                        GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(15));
    }

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

    glUseProgram(0);
    glDeleteProgram(progID);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}