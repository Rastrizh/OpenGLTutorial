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
	Camera(const glm::vec3& pos = { 0.f, 0.f, 6.f }, const glm::vec3& top = { 0.f, 1.f, 0.f });

	glm::mat4 GetViewMatrix();
	void updateProjectionMatrix(float fov, float aspect_ratio, float near = 0.1f, float far = 100.f);
	glm::mat4 GetProjectionMatrix() const;

	glm::vec3 const GetCameraPosition() const;
	glm::vec3 const GetCameraDirection() const;

protected:
	void updateTarget(float yaw, float pitch);

	void updateCameraVectors();

};

class CameraController : public Camera
{
public:
	static float YAW;
	static float PITCH;
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
	CameraController();

	void processKeyboard(GLFWwindow* window, float deltaTime);

	void processMouse(double xpos, double ypos);

	void ProcessMouseScroll(double yoffset, float aspect_ratio);

	float GetCameraFOV() const;
};

#endif
