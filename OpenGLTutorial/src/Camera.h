#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(0.f, 0.f, 3.f), const glm::vec3& front = glm::vec3(0.f, 0.f, -1.f), const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f), float speed = 0.05f)
		:
		m_cameraSpeed(speed),
		m_cameraPosition(pos),
		m_cameraFront(front),
		m_cameraUp(up)
	{
		SetCameraView();
	}
	~Camera();

	void inline SetCameraSpeed(const float& speed) { m_cameraSpeed = speed; }
	void inline SetCameraPosition(const glm::vec3& pos) { m_cameraPosition = pos; }
	void inline SetCameraFront(const glm::vec3& front) { m_cameraFront = front; }
	void inline SetCameraUp(const glm::vec3& up) { m_cameraUp = up; }

	float inline GetCameraSpeed() const { return m_cameraSpeed; }
	glm::vec3 inline GetCameraPosition() const { return m_cameraPosition; }
	glm::vec3 inline GetCameraFront() const { return m_cameraFront; }
	glm::vec3 inline GetCameraUp() const { return m_cameraUp; }

	void SetCameraView() { m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp); }
	glm::mat4 GetCameraView() const { return m_view; }

private:
	float		m_cameraSpeed;
	glm::vec3	m_cameraPosition;
	glm::vec3	m_cameraFront;
	glm::vec3	m_cameraUp;
	glm::mat4	m_view;
};

Camera::~Camera()
{
}

#endif
