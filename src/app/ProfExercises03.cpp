#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
using namespace std;

vector<GLfloat> vertOnly = {
    -0.3f, -0.3f, 0.0f,
    +0.3f, -0.3f, 0.0f,
    -0.3f, +0.3f, 0.0f,

    -0.3f, +0.3f, 0.0f,
    +0.3f, -0.3f, 0.0f,
    +0.3f, +0.3f, 0.0f,
};

static void error_callback(int error, const char* desc) {
    cout << "ERROR " << error << ": " << desc << endl;
}

int main(int argc, char **argv) {
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

    glewExperimental = true;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        cout << "GLEW ERROR: ";
        cout << glewGetErrorString(err) << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    int fwidth = 0;
    int fheight = 0;

    glClearColor(0.8f, 0.0f, 0.1f, 1.0f);

    GLuint progID = 0;

    try {
        string vertCode = readFileToString("./shaders/ProfExercises03/Basic.vs");
        string fragCode = readFileToString("./shaders/ProfExercises03/Basic.fs");

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

    // Setup VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(   GL_ARRAY_BUFFER, vertOnly.size()*sizeof(GLfloat), 
                    vertOnly.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Setup VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3*sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);











    // Render loop
    while(!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &fwidth, &fheight);
        glViewport(0, 0, fwidth, fheight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(progID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(15));
    }

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);

    glUseProgram(0);
    glDeleteProgram(progID);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}