#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<std::shared_ptr<Texture2D>> m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
	void Draw(Shader shader);

private:
	std::shared_ptr<VertexArray> VAO;

	void SetupMesh();
};
#endif // MESH_H