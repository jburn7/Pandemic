#pragma once
#include "event.h"

enum class Key
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	A,
	D,
	SPACE
};

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(Key key) : Event(EventType::KEY_PRESSED_EVENT)
	{
		mKey = key;
	}
	~KeyPressedEvent() {}

	Key getKey() const { return mKey; }
private:
	Key mKey;
};

class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(Key key) : Event(EventType::KEY_RELEASED_EVENT)
	{
		mKey = key;
	}
	~KeyReleasedEvent() {}

	Key getKey() const { return mKey; }
private:
	Key mKey;
};