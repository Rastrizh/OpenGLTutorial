#ifndef BUFFER_H
#define BUFFER_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class VertexBuffer
{
private:
	unsigned int ID;
public:
	VertexBuffer(size_t size);
	VertexBuffer(std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, size_t size);
};

class IndexBuffer
{
private:
	unsigned int ID;
	size_t m_count;
public:
	IndexBuffer(unsigned int* indices, size_t count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, size_t size);

	inline size_t GetCount() const { return m_count; }
};

#endif //BUFFER_H