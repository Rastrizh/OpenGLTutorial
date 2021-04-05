#ifndef MESH_H
#define MESH_H

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

enum TexType
{
	diffuse,
	specular,
	normal,
	hight
};

struct Texture
{
	Texture2D texture;
	TexType type;
};

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	Mesh& operator=(Mesh&&) = delete;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(Shader shader);

private:
	unsigned int VBO, EBO, VAO;

	void SetupMesh();
};
#endif // MESH_H