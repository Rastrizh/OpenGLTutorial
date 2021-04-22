#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Buffer.h"

enum DepthAttachmentType
{
	RBO,
	TEXTURE
};

class FrameBuffer
{
private:
	unsigned int ID;
	std::vector<unsigned int> m_colorAttachments;
	unsigned int m_depthAttachment;
	unsigned int quadVAO;
	std::shared_ptr<VertexBuffer> quadVBO;
public:
	FrameBuffer(unsigned int numColorAttachments);
	~FrameBuffer();
	void Initialize(unsigned int numColorAttachments);
	void Bind();
	void Unbind();
	void Draw();
private:
	void CreateTextures(unsigned int* id, unsigned int count);
	void BindTexture(unsigned int id);	
	void AttachColorTexture(unsigned int id, GLenum format, unsigned int width, unsigned int height, int index);
	void AttachDepth(unsigned int id, GLenum format, unsigned int width, unsigned int height, GLenum attachmentType, DepthAttachmentType depthAttachmentType = TEXTURE);
	void CreateRenderBuffer(unsigned int* id);
	void BindRenderBuffer(unsigned int id);
	void CreateScreenQuad();
};

#endif // FRAMEBUFFER_H