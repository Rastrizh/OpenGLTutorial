#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vendor/stb_image/stb_image.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Events/Event.h"
#include "Texture.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model/Model.h"
#include "FrameBuffer.h"
#include "CubeMap.h"

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 768;

float currentFrame = 0.f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightDirection(-0.4f, -1.0f, -0.5f);
glm::vec3 lightPosition(1.0f, 1.0f, 3.0f);

int main()
{
	Window* window = Window::GetInstance();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);

	CameraController cameraController;

	Shader lightingShader("shaders/testVert.vs", "shaders/MultipleLights.fs");
	Shader skyboxShader("shaders/skyboxShader.vs", "shaders/skyboxShader.fs");
	Shader screenShader("shaders/framebuffers_screen.vs", "shaders/framebuffers_screen.fs");

	Model ourModel("../contents/assets/crytek-sponza-huge-vray-obj/crytek-sponza-huge-vray.obj");

	std::vector<std::string> faces
	{
		"../contents/textures/skybox/right.jpg",
		"../contents/textures/skybox/left.jpg",
		"../contents/textures/skybox/top.jpg",
		"../contents/textures/skybox/bottom.jpg",
		"../contents/textures/skybox/front.jpg",
		"../contents/textures/skybox/back.jpg"
	};
	Cubemap skybox(faces);

	lightingShader.Use();
	lightingShader.SetFloat("material.shininess", 64.0f);
	
	lightingShader.setVec3("directionlight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.setVec4("directionlight.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
	lightingShader.setVec4("directionlight.diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
	lightingShader.setVec4("directionlight.specular", 0.6f, 0.6f, 0.6f, 1.0f);

	screenShader.Use();
	screenShader.SetInt("screenTexture", 0);

	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0);

	FrameBuffer framebuffer(1);

	while (!glfwWindowShouldClose(window->GetNativeWindow()))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		cameraController.processKeyboard(window->GetNativeWindow(), deltaTime);

		framebuffer.Bind();
		glEnable(GL_DEPTH_TEST); // ��������� ������ ����� ������� 

		// ����������, ��� �������� ���������� �����������
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lightingShader.Use();
		lightingShader.setVec3("cameraPos", cameraController.GetCameraPosition());

		cameraController.updateProjectionMatrix(cameraController.GetCameraFOV(), (float)WIDTH / (float)HEIGHT);
		glm::mat4 projection = cameraController.GetProjectionMatrix();
		glm::mat4 view = cameraController.GetViewMatrix();

		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		lightingShader.setMat4("model", model);
		ourModel.Draw(lightingShader);

		glDepthFunc(GL_LEQUAL); // ������ ������� �������, ����� ���������� ����������� ����� �������, ����� �������� ����� ����������� ������ �������
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(cameraController.GetViewMatrix())); // ������� �� ������� ���� ������, ���������� �� �������� ����������
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		skybox.DrawSkybox();
		glDepthFunc(GL_LESS); // ��������������� ����������� �������� ������� ����� �������

		// ������ ����� ������������ � �����������, ��������� �� ��������� � ������������ ������������� � ������������� �������� ��������� �����������
		framebuffer.Unbind();
		glDisable(GL_DEPTH_TEST); // ��������� ����� ����� �������. ������ �������� ������������� �� ����� ���������� � ���������� ����������� ������� �����

		// ������� ��� ������������� ������
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ������������� ���� ������� �� "�����" (���������� ���������� ���� �� ����� (�� ����� ���� ��� �� �����������, ��� ��� �� ��� ����� �� ������ ������ ������������ �� ���������������))
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Use();
		framebuffer.Draw();

		window->OnUpdate();		
	}

	return 0;
}
