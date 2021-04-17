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

	Model ourModel("../contents/assets/crytek-sponza-huge-vray-obj/crytek-sponza-huge-vray.obj");

	lightingShader.Use();
	lightingShader.SetFloat("material.shininess", 64.0f);
	
	lightingShader.setVec3("directionlight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.setVec3("directionlight.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("directionlight.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("directionlight.specular", 0.6f, 0.6f, 0.6f);

	while (!glfwWindowShouldClose(window->GetNativeWindow()))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		cameraController.processKeyboard(window->GetNativeWindow(), deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
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

		window->OnUpdate();		
	}

	return 0;
}
