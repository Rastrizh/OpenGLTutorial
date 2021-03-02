#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Window.h"

Window::Window(const unsigned int width, const unsigned int height, const char* title)
	:
	m_width(width), m_height(height), m_title(title)
{
	Init();
}

Window::~Window()
{

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

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		Window* w_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		w_window->framebuffer_size_callback(window, width, height);
	});
}

void Window::ShutDown()
{
	glfwTerminate();
}
