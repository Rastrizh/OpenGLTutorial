#ifndef WINDOW_H
#define WINDOW_H

class Window
{
private:
	GLFWwindow* m_window;
	const char* m_title;
	unsigned int m_width;
	unsigned int m_height;
	bool isVSync = false;

	static Window* s_window;
public:
	GLCore::event<int, int> framebuffer_size_event;
public:
	Window(const unsigned int width = 1024, const unsigned int height = 720, const char* title = "OpenGLTutorial");
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
	static inline Window* GetInstance() { return s_window; }
	void Init();
	void ShutDown();
	void OnUpdate();

	inline const char* GetTitle() const { return m_title; }
	inline unsigned int& GetWidth() { return m_width; }
	inline unsigned int& GetHeight() { return m_height; }
	inline GLFWwindow* GetNativeWindow() { return m_window; }
	inline bool IsVSync() const { return isVSync; }
	void SetVSync(bool enabled);

	void OnFramebufferSize(int width, int hight) { glViewport(0, 0, width, hight); }
};

#endif // WINDOW_H

