#ifndef INPUT_H
#define INPUT_H
#include "Events/KeyEvents.h"

class Input
{
protected:
	Input() = default;

public:
	GLCore::event<double, double> mouse_moved_event;
	GLCore::event<double, float> mouse_scrolled_event;

public:
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	static inline Input* GetInstance() { return s_instance; }

	static bool IsKeyPressed(int key_code);
	static bool IsMouseButtonPressed(int button);
	static std::pair<double, double> GetMousePosition();
	static double GetMouseX();
	static double GetMouseY();
private:
	static Input* s_instance;
};

#endif // INPUT_H