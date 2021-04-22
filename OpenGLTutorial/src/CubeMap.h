#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include "Buffer.h"

class Cubemap
{
private:
	std::shared_ptr<VertexBuffer> cubeVBO;
	unsigned int cubeVAO;
	unsigned int textureID;
public:
	Cubemap(std::vector<std::string>& faces);
	unsigned int LoadCubemap(std::vector<std::string>& faces);
	void createCube();
	void DrawSkybox();
};

#endif // CUBE_MAP_H