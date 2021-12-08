#pragma once
#include "event.h"

enum Key
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(EventType type, Key key) : Event(type)
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
	KeyReleasedEvent(EventType type, Key key) : Event(type)
	{
		mKey = key;
	}
	~KeyReleasedEvent() {}

	Key getKey() const { return mKey; }
private:
	Key mKey;
};