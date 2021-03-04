#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Events/KeyEvents.h"
#include "Window.h"
#include "Input.h"

Window* Window::s_window = new Window();

Window::Window(const unsigned int width, const unsigned int height, const char* title)
	:
	m_width(width), m_height(height), m_title(title)
{
	Init();
}

Window::~Window()
{
	ShutDown();
}

void Window::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	framebuffer_size_event += [this](int width, int height) {
		OnFramebufferSize(width, height);
	};

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		Window* w_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		w_window->framebuffer_size_event(width, height);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		Input::GetInstance()->mouse_moved_event(xpos, ypos);
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
		Input::GetInstance()->mouse_scrolled_event(yoffset, (float)(Window::GetInstance()->GetWidth() / Window::GetInstance()->GetHeight()));
	});
}

void Window::ShutDown()
{
	glfwDestroyWindow(m_window);
}

void Window::OnUpdate()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	isVSync = enabled;
}
