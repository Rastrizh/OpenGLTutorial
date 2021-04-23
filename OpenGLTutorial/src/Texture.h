#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "vendor/stb_image/stb_image.h"
#include <string>

enum TexType
{
	diffuse = 1,
	specular = 2,
	height = 5,
	normal = 6
};

class Texture2D
{
public:
	TexType m_type;
	std::string m_filename;
	std::string m_path;

private:
	stbi_uc* m_data = nullptr;
	unsigned int id;
	int m_width;
	int m_height;
	int m_nrChannels;
	GLenum m_format = GL_RGBA;
public:
	Texture2D(unsigned int width, unsigned int hight);
	Texture2D(const char* path);
	~Texture2D();

	bool operator==(const Texture2D& other) const { return id == other.id; }

	inline GLenum GetFormat() const { return m_format; }
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline unsigned int GetId() const { return id; }

	void CreateTexture(const char* path);
	void Bind();
};

#endif // TEXTURE_H
