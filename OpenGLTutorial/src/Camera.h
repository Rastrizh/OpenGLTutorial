#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

float YAW = -90.0f;
float PITCH = 0.0f;

class Camera
{
protected:
	glm::vec3 m_position;
	glm::vec3 m_top;
	glm::vec3 m_target;
	glm::vec3 m_right;

	glm::vec3 m_cameraTop;
	glm::mat4 m_cameraViewMatrix;
	glm::mat4 m_cameraProjectionMatrix;

public:
	Camera(const glm::vec3& pos = { 0.f, 0.f, 3.f }, const glm::vec3& top = { 0.f, 1.f, 0.f })
		:
		m_target(0.f, 0.f, -1.f)
	{
		m_position = pos;
		m_top = top;
		updateTarget(YAW, PITCH);
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() { return m_cameraViewMatrix = glm::lookAt(m_position, m_position + m_target, m_cameraTop); }
	void updateProjectionMatrix(float fov, float aspect_ratio, float near = 0.1f, float far = 100.f)
	{
		m_cameraProjectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
	}
	glm::mat4 GetProjectionMatrix() const { return m_cameraProjectionMatrix; }

public:
	void updateTarget(float yaw, float pitch)
	{
		glm::vec3 target;
		target.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		target.y = sin(glm::radians(pitch));
		target.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_target = glm::normalize(target);
	}

	void updateCameraVectors()
	{		
		m_right = glm::normalize(glm::cross(m_target, m_top));
		m_cameraTop = glm::normalize(glm::cross(m_right, m_target));
	}

};

class CameraController : public Camera
{
private:
	float cameraSpeed		= 10.0f;
	float cameraSensetivity = 0.05f;
	float cameraZoom		= 0.0025f;
	float cameraFOV			= 45.0f;
	float pitch;
	float yaw;

	bool  firstMouse;
	float lastX;
	float lastY;

public:
	CameraController()
	{
		firstMouse = true;
		yaw = YAW;
		pitch = PITCH;
	}

	void processKeyboard(GLFWwindow* window, float deltaTime)
	{
		float cameraVelocity = cameraSpeed * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_position += m_target * cameraVelocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_position -= m_target * cameraVelocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_position -= m_right * cameraVelocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_position += m_right * cameraVelocity;

		updateTarget(yaw, pitch);
		updateCameraVectors();
	}

	void processMouse(double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= cameraSensetivity;
		yoffset *= cameraSensetivity;
		
		yaw += xoffset;
		pitch += yoffset;
		
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		
		updateTarget(yaw, pitch);
		updateCameraVectors();
	}

	void ProcessMouseScroll(double yoffset, float aspect_ratio)
	{
		if (cameraFOV > 1.0f && cameraFOV <= 45.0f)
			cameraFOV -= yoffset;
		else if (cameraFOV <= 1.0f)
			cameraFOV = 1.0f;
		else if (cameraFOV > 45.0f)
			cameraFOV = 45.0f;
	}

	float GetCameraFOV() const { return cameraFOV; }
};

#endif
