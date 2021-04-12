#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "vendor/stb_image/stb_image.h"

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
	unsigned int id;
	int m_width;
	int m_height;
	int m_nrChannels;
	GLenum m_format = GL_RGB;

	TexType m_type;
	std::string m_path;

private:
	unsigned char* m_data;
public:
	Texture2D()
	{
		glGenTextures(1, &id);
	}

	Texture2D(const char* path)
	{
		glGenTextures(1, &id);
		CreateTexture(path);
	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void SetMipping(GLint method)
	{
		Bind();
		if (method == GL_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
	}
	void SetMiping(GLenum axis, GLint method)
	{
		Bind();
		if (method == GL_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		glTexParameteri(GL_TEXTURE_2D, axis, method);
	}
	void SetFiltering(GLint method)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, method);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, method);
	}
	void SetFiltering(GLenum level, GLint method)
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, level, method);
	}
	
	void CreateTexture(const char* path)
	{
		m_data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
		if (m_data)
		{
			if (m_nrChannels == 1)
				m_format = GL_RED;
			else if (m_nrChannels == 3)
				m_format = GL_RGB;
			else if (m_nrChannels == 4)
				m_format = GL_RGBA;

			Generate();

			SetMipping(GL_REPEAT);
			SetFiltering(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			SetFiltering(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			stbi_image_free(m_data);
		}
		else
		{
			// throw an exception
			stbi_image_free(m_data);
		}
	}

	void Load(const char* path)
	{
		m_data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
	}

private:
	void Generate()
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
};

#endif // TEXTURE_H
