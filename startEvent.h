#pragma once
#include "event.h"

class StartEvent : public Event
{
public:
	StartEvent(EventType type) : Event(type) {}
	~StartEvent() {}
};