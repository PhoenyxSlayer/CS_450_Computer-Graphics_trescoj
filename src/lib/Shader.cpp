#include "Shader.hpp"

// Read from file and dump in string
string readFileToString(string filename) {
	// Open file
	ifstream file(filename);
	// Could we open file?
	if(!file || file.fail()) {
		cerr << "ERROR: Could not open file: " << filename << endl;
		const char *m = ("ERROR: Could not open file: " + filename).c_str();
		throw runtime_error(m);
	}

	// Create output stream to receive file data
	ostringstream outS;
	outS << file.rdbuf();
	// Get actual string of file contents
	string allS = outS.str();
	// Close file
	file.close();
	// Return string
	return allS;
}

// Print out shader code
void printShaderCode(string &vertexCode, string &fragCode) {
	cout << "***********************" << endl;
	cout <<"** VERTEX SHADER CODE **" << endl;
	cout << "***********************" << endl;
	cout << vertexCode << endl;
	cout << "*************************" << endl;
	cout <<"** FRAGMENT SHADER CODE **" << endl;
	cout << "*************************" << endl;
	cout << fragCode << endl;
	cout << "*************************" << endl;
}

// GLSL Compiling/Linking Error Check
// Returns GL_TRUE if compile was successful; GL_FALSE otherwise.
GLint checkGLSLError(GLuint ID, bool isCompile) {

	GLint resultGL = GL_FALSE;
	int infoLogLength;
	char *errorMessage = nullptr;

	if(isCompile) {
		// Get the compilation status and message length
		glGetShaderiv(ID, GL_COMPILE_STATUS, &resultGL);	
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else {
		// Get linking status and message length
		glGetProgramiv(ID, GL_LINK_STATUS, &resultGL);
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	// Make sure length is at least one and allocate space for message	
	infoLogLength = (infoLogLength > 1) ? infoLogLength : 1;
	errorMessage = new char[infoLogLength];	

	// Get actual message
	if(isCompile)
		glGetShaderInfoLog(ID, infoLogLength, NULL, errorMessage);		
	else	
		glGetProgramInfoLog(ID, infoLogLength, NULL, errorMessage);

	// Print error message
	if(infoLogLength > 1)
		cout << errorMessage << endl;

	// Cleanup
	if(errorMessage) delete [] errorMessage;

	// Return OpenGL error
	return resultGL;
}

// Creates and compiles GLSL shader from code string; returns shader ID
GLuint createAndCompileShader(const char *shaderCode, GLenum shaderType) {
	// Create the shader ID
	GLuint shaderID = glCreateShader(shaderType);

	// Compile the vertex shader...
	cout << "Compiling shader..." << endl;
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	// Checking result of compilation...
	GLint compileOK = checkGLSLError(shaderID, true);
	if (!compileOK || shaderID == 0) {
		glDeleteShader(shaderID);		
		cout << "Error compiling shader." << endl;
		throw runtime_error("Error compiling shader.");
	}

	// Return shader ID
	return shaderID;
}

// Given a list of compiled shaders, create and link a shader program (ID returned).
GLuint createAndLinkShaderProgram(std::vector<GLuint> allShaderIDs) {

	// Create program ID and attach shaders
	cout << "Linking program..." << endl;
	GLuint programID = glCreateProgram();
	for (GLuint &shaderID : allShaderIDs) {
		glAttachShader(programID, shaderID);
	}

	// Actually link the program
	glLinkProgram(programID);

	// Detach shaders (program already linked, successful or not)
	for (GLuint &shaderID : allShaderIDs) {
		glDetachShader(programID, shaderID);		
	}

	// Check linking
	GLint linkOK = checkGLSLError(programID, false);
	if (!linkOK || programID == 0) {		
		glDeleteProgram(programID);		
		cout << "Error linking shaders." << endl;
		throw runtime_error("Error linking shaders.");
	}

	// Return program ID
	return programID;
}

// Does the following:
// - Creates and compiles vertex and fragment shaders (from provided code strings)
// - Creates and links shader program
// - Deletes vertex and fragment shaders
GLuint initShaderProgramFromSource(string vertexShaderCode, string fragmentShaderCode) {
	GLuint vertID = 0;
	GLuint fragID = 0;
	GLuint programID = 0;

	try {
		// Create and compile shaders
		cout << "Vertex shader: ";
		vertID = createAndCompileShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
		cout << "Fragment shader: ";
		fragID = createAndCompileShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

		// Create and link program
		programID = createAndLinkShaderProgram({ vertID, fragID });

		// Delete individual shaders
		glDeleteShader(vertID);
		glDeleteShader(fragID);

		// Success!
		cout << "Program successfully compiled and linked!" << endl;
	}
	catch (exception e) {
		// Cleanup shaders and shader program, just in case
		if (vertID) glDeleteShader(vertID);
		if (fragID) glDeleteShader(fragID);		
		// Rethrow exception
		throw e;
	}

	return programID;
}
