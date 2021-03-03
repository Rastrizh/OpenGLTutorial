#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include "Event.h"

template<class... TArgs>
class KeyEvent : public GLCore::event<TArgs...>
{
private:
	int m_key_code;

public:
	inline int GetKeyCode() const { return m_key_code; }

protected:
	KeyEvent(int key_code)
		: 
		m_key_code(key_code) 
	{}

};

class KeyPressedEvent : public KeyEvent<int, int>
{
private:
	int m_repeat_count;
public:
	KeyPressedEvent(int key_code, int repeat_count)
		:
		KeyEvent(key_code),
		m_repeat_count(repeat_count)
	{}

	inline int GetRepeatCount() const { return m_repeat_count; }
};

class KeyReleasedEvent : public KeyEvent<int>
{
public:
	KeyReleasedEvent(int key_code)
		:
		KeyEvent(key_code)
	{}
};

#endif // KEY_EVENTS_H