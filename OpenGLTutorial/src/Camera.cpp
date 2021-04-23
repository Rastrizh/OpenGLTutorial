#include "Camera.h"
#include "Input.h"

Camera::Camera(const glm::vec3& pos /*= { 0.f, 0.f, 6.f }*/, const glm::vec3& top /*= { 0.f, 1.f, 0.f }*/) :
	m_target(0.f, 0.f, -1.f)
{
	m_position = pos;
	m_top = top;
	updateTarget(CameraController::YAW, CameraController::PITCH);
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return m_cameraViewMatrix = glm::lookAt(m_position, m_position + m_target, m_cameraTop);
}

void Camera::updateProjectionMatrix(float fov, float aspect_ratio, float near /*= 0.1f*/, float far /*= 100.f*/)
{
	m_cameraProjectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_cameraProjectionMatrix;
}

glm::vec3 const Camera::GetCameraPosition() const
{
	return m_position;
}

glm::vec3 const Camera::GetCameraDirection() const
{
	return m_target;
}

void Camera::updateTarget(float yaw, float pitch)
{
	glm::vec3 target;
	target.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	target.y = sin(glm::radians(pitch));
	target.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_target = glm::normalize(target);
}

void Camera::updateCameraVectors()
{

	m_right = glm::normalize(glm::cross(m_target, m_top));
	m_cameraTop = glm::normalize(glm::cross(m_right, m_target));
}

CameraController::CameraController()
{
	firstMouse = true;
	yaw = YAW;
	pitch = PITCH;

	Input::GetInstance()->mouse_moved_event += [this](double xpos, double ypos) {
		processMouse(xpos, ypos);
	};

	Input::GetInstance()->mouse_scrolled_event += [this](double yoffset, float aspect_ratio) {
		ProcessMouseScroll(yoffset, aspect_ratio);
	};
}

void CameraController::processKeyboard(GLFWwindow* window, float deltaTime)
{
	float cameraVelocity = cameraSpeed * deltaTime;

	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (Input::IsKeyPressed(GLFW_KEY_W))
		m_position += m_target * cameraVelocity;

	if (Input::IsKeyPressed(GLFW_KEY_S))
		m_position -= m_target * cameraVelocity;

	if (Input::IsKeyPressed(GLFW_KEY_A))
		m_position -= m_right * cameraVelocity;

	if (Input::IsKeyPressed(GLFW_KEY_D))
		m_position += m_right * cameraVelocity;

	updateTarget(yaw, pitch);
	updateCameraVectors();
}

void CameraController::processMouse(double xpos, double ypos)
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

void CameraController::ProcessMouseScroll(double yoffset, float aspect_ratio)
{
	if (cameraFOV > 1.0f && cameraFOV <= 45.0f)
		cameraFOV -= yoffset;
	else if (cameraFOV <= 1.0f)
		cameraFOV += 1.0f;
	else if (cameraFOV > 45.0f)
		cameraFOV = 45.0f;
}

float CameraController::GetCameraFOV() const
{
	return cameraFOV;
}

float CameraController::YAW = -90.0f;
float CameraController::PITCH = 0.0f;