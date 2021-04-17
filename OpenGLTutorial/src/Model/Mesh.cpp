#include <vector>
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures)
	:
	m_vertices(vertices),
	m_indices(indices),
	m_textures(textures)
{
	VAO = std::make_shared<VertexArray>();
	SetupMesh();
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffNum = 0;
	unsigned int specNum = 0;
	unsigned int normalNum = 0;
	unsigned int hightNum = 0;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		switch (m_textures[i].m_type)
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
		m_textures[i].Bind();
	}
	glActiveTexture(GL_TEXTURE0);
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	VAO->Unbind();
}

void Mesh::SetupMesh()
{
	VAO->Bind();
	
	VertexBuffer VBO(m_vertices);
	IndexBuffer EBO(&m_indices[0], m_indices.size());

	VAO->AddVertexBuffer(VBO);
	VAO->SetIndexBuffer(EBO);

	VAO->Unbind();
}
