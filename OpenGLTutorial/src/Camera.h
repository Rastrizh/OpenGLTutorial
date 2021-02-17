#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

float lastX = 0.f;
float lastY = 0.f;
bool firstMouse = true;

class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(0.f, 0.f, 3.f))
		:
		m_cameraPosition(pos),
		m_cameraFront(glm::vec3(0.f, 0.f, -1.f)),
		m_cameraUp(glm::vec3(0.f, 1.f, 0.f))
	{
		UpdateCameraVectors();
	}
	~Camera(){}


	void ProcessKeyboardInput(const Direction& dir, const float& deltaTime)
	{
		SetCameraSpeed(9.5f * deltaTime);

		switch (dir)
		{
		case FORWARD:
			m_cameraPosition += m_cameraFront * m_cameraSpeed;
			break;
		case BACKWARD:
			m_cameraPosition -= m_cameraFront * m_cameraSpeed;
			break;
		case LEFT:
			m_cameraPosition -= m_cameraRight * m_cameraSpeed;
			break;
		case RIGHT:
			m_cameraPosition += m_cameraRight * m_cameraSpeed;
		default:
			break;
		}
		UpdateCameraVectors();
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= m_sensitivity;
		yoffset *= m_sensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		UpdateCameraVectors();
	}

	void processScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (fov > 1.0f && fov < 45.0f)
			fov -= yoffset;
		else if (fov <= 1.0f)
			fov = 1.0f;
		else if (fov >= 45.0f)
			fov = 45.0f;
	}

private:
	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_cameraFront = glm::normalize(front);

		m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
	}

public:
	void inline SetCameraSpeed(const float& speed) { m_cameraSpeed = speed; }
	void inline SetCameraPosition(const glm::vec3& pos) { m_cameraPosition = pos; }
	void inline SetCameraFront(const glm::vec3& front) { m_cameraFront = front; }
	void inline SetCameraUp(const glm::vec3& up) { m_cameraUp = up; }

	float inline GetCameraSpeed() const { return m_cameraSpeed; }
	float inline GetCameraFOV() const { return fov; }
	glm::vec3 inline GetCameraPosition() const { return m_cameraPosition; }
	glm::vec3 inline GetCameraFront() const { return m_cameraFront; }
	glm::vec3 inline GetCameraUp() const { return m_cameraUp; }

	glm::mat4& GetCameraView() { return m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp); }

private:
	glm::vec3	m_cameraPosition;
	glm::vec3	m_cameraFront;
	glm::vec3	m_cameraUp;
	glm::vec3	m_cameraRight;
	glm::vec3	m_cameraDirection;

	float		m_cameraSpeed	= 9.0f;
	float		m_Yaw			= -90.0f;
	float		m_Pitch			= 0.0f;
	float		m_sensitivity	= 0.05f;
	float		fov				= 45.0f;

	glm::mat4	m_view;
};

#endif
