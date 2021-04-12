#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture2D> loaded_textures;

public:
	Model(const char* path) { LoadModel(path); }
	void Draw(Shader& shader);

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture2D> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, TexType typeName);
};

#endif // MODEL_H