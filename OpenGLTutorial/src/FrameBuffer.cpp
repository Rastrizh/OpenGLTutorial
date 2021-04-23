#include <vector>
#include <glad/glad.h>
#include <iostream>
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned int numColorAttachments)
{
	Initialize(numColorAttachments);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &ID);
	glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
	glDeleteRenderbuffers(1, &m_depthAttachment);
	//glDeleteTextures(1, &m_depthAttachment);
	glDeleteVertexArrays(1, &quadVAO);
}

void FrameBuffer::Initialize(unsigned int numColorAttachments)
{
	if (ID)
	{
		glDeleteFramebuffers(1, &ID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);

		m_colorAttachments.clear();
		m_depthAttachment = 0;
	}

	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	m_colorAttachments.resize(numColorAttachments);
	CreateTextures(m_colorAttachments.data(), m_colorAttachments.size());

	for (unsigned int i = 0; i < m_colorAttachments.size(); i++)
	{
		BindTexture(m_colorAttachments[i]);
		AttachColorTexture(m_colorAttachments[i], GL_RGB, 1024, 720, i);
	}
	CreateRenderBuffer(&m_depthAttachment);
	BindRenderBuffer(m_depthAttachment);
	AttachDepth(m_depthAttachment, GL_DEPTH24_STENCIL8, 1024, 720, GL_DEPTH_STENCIL_ATTACHMENT, DepthAttachmentType::RBO);

	CreateScreenQuad();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, 1024, 720);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Draw()
{
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachments[0]); // используем прикрепленную цветовую текстуру в качестве текстуры для прямоугольника
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void FrameBuffer::CreateTextures(unsigned int* id, unsigned int count)
{
	glGenTextures(count, id);
}

void FrameBuffer::BindTexture(unsigned int id)
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void FrameBuffer::AttachColorTexture(unsigned int id, GLenum format, unsigned int width, unsigned int height, int index)
{
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
}

void FrameBuffer::AttachDepth(unsigned int id, GLenum format, unsigned int width, unsigned int height, GLenum attachmentType, DepthAttachmentType depthAttachmentType)
{
	switch (depthAttachmentType)
	{
	case RBO:
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, id);
		break;
	case TEXTURE:
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
		break;
	}
}

void FrameBuffer::CreateRenderBuffer(unsigned int* id)
{
	glGenRenderbuffers(1, id);
}

void FrameBuffer::BindRenderBuffer(unsigned int id)
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void FrameBuffer::CreateScreenQuad()
{
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	quadVBO = std::make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
