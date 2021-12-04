#pragma once
#include "event.h"

enum Key
{
};

class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(EventType type, Key key) : Event(type)
	{

	}
	~KeyReleasedEvent() {}

	Key getKey() const { return mKey; }
private:
	Key mKey;
};