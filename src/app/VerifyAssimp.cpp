#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>   
using namespace std;

// Main 
int main(int argc, char **argv) {
	// Verifying that Assimp works correctly
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("sampleModels/teapot.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cerr << "Error: " << importer.GetErrorString() << endl;
		exit(1);
	}
    cout << "Number of meshes in teapot: " << scene->mNumMeshes << endl;
		
	return 0;
}
