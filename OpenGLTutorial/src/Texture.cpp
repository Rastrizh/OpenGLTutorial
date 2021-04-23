#include "Texture.h"

Texture2D::Texture2D(const char* path)
{
	glGenTextures(1, &id);
	CreateTexture(path);
}

Texture2D::Texture2D(unsigned int width, unsigned int hight)
{
	glGenTextures(1, &id);

	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, m_data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &id);
}

void Texture2D::CreateTexture(const char* path)
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

		Bind();

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(m_data);
		m_data = nullptr;
	}
	else
	{
		// throw an exception
		stbi_image_free(m_data);
		m_data = nullptr;
	}
}
