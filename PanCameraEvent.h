#pragma once
#include "event.h"
#include "Vector2D.h"

class PanCameraEvent : public Event
{
public:
	PanCameraEvent(EventType type, const Vector2D delta) : Event(type)
	{
		this->delta = delta;
	}

	const Vector2D &getDelta() const { return delta; }

private:
	Vector2D delta;
};