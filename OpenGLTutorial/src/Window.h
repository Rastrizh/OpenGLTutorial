#ifndef WINDOW_H
#define WINDOW_H

class Window
{
public:
	GLFWwindow* m_window;
private:
	const char* m_title;
	unsigned int m_width;
	unsigned int m_height;
public:
	Window(const unsigned int width, const unsigned int height, const char* title);
	~Window();
	void Init();
	void ShutDown();

	inline const char* GetTitle() const { return m_title; }
	inline unsigned int& GetWidth() { return m_width; }
	inline unsigned int& GetHeight() { return m_height; }

	void framebuffer_size_callback(GLFWwindow* window, int width, int hight) { glViewport(0, 0, width, hight); }
};

#endif // WINDOW_H
