#pragma once
#include "event.h"

class UpdateFPSEvent : public Event
{
public:
	UpdateFPSEvent(EventType type, int fps);
	~UpdateFPSEvent();

	int getFPS() const;
protected:
	int mFPS;
};