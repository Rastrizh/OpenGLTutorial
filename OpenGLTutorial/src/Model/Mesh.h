#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "../Shader.h"
#include "../Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

enum TexType
{
	diffuse = 1,
	specular = 2,
	height = 5,
	normal = 6
};

struct Texture
{
	Texture2D texture;
	TexType type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(Shader shader);

private:
	unsigned int VBO, EBO, VAO;

	void SetupMesh();
};
#endif // MESH_H