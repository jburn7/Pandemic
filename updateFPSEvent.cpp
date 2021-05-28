#include "updateFPSEvent.h"

UpdateFPSEvent::UpdateFPSEvent(EventType type, int fps) : Event(type)
{
	mFPS = fps;
}

UpdateFPSEvent::~UpdateFPSEvent()
{
}

int UpdateFPSEvent::getFPS() const
{
	return mFPS;
}
