#include <vector>
#include "glm/glm.hpp"
#include "../Shader.h"
#include "../Texture.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
	:
	m_vertices(vertices),
	m_indices(indices),
	m_textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffNum = 1;
	unsigned int specNum = 1;
	unsigned int normalNum = 1;
	unsigned int hightNum = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		switch (m_textures[i].type)
		{
		case 0:
			shader.SetFloat("material.diffuse[diffNum]", i);
			diffNum++;
			break;
		case 1:
			shader.SetFloat("material.specular[specNum]", i);
			specNum++;
			break;
		case 2:
			shader.SetFloat("material.normal[normalNum]", i);
			normalNum++;
			break;
		case 3:
			shader.SetFloat("material.hight[hightNum]", i);
			hightNum++;
			break;
		default:
			break;
		}
		m_textures[i].texture.Bind();
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), (void*)&m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), (void*)&m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}
