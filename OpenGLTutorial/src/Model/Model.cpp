#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"

void Model::Draw(Shader& shader)
{
	for (auto &a : meshes)
		a.Draw(shader);
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture2D> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture2D> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TexType::specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture2D> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TexType::normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture2D> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TexType::height);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture2D> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, TexType typeName)
{
	std::vector<Texture2D> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(loaded_textures[j].m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::string filename(str.C_Str());
			filename = directory + '/' + filename;

			Texture2D texture(filename.c_str());
			texture.m_path = str.C_Str();
			texture.m_type = typeName;
			textures.push_back(texture);
			loaded_textures.push_back(texture);
		}
	}
	return textures;
}
