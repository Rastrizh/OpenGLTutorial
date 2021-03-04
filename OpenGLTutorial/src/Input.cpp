#include <utility>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Window.h"

Input* Input::s_instance = new Input();

bool Input::IsKeyPressed(int key_code)
{
	GLFWwindow* window = Window::GetInstance()->GetNativeWindow();
	auto state = glfwGetKey(window, key_code);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	GLFWwindow* window = Window::GetInstance()->GetNativeWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

std::pair<double, double> Input::GetMousePosition()
{
	GLFWwindow* window = Window::GetInstance()->GetNativeWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { xpos, ypos };
}

double Input::GetMouseX()
{
	auto[x, y] = GetMousePosition();
	return x;
}

double Input::GetMouseY()
{
	auto[x, y] = GetMousePosition();
	return y;
}
