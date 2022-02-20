#pragma once
#include "event.h"
#include "Vector2D.h"

class PanCameraEvent : public Event
{
public:
	PanCameraEvent(EventType type, const Vector2D delta) : Event(type)
	{
		mDelta = delta;
	}

	const Vector2D &getDelta() const { return mDelta; }

private:
	Vector2D mDelta;
};

class ZoomCameraEvent : public Event
{
public:
	ZoomCameraEvent(EventType type, const double delta) : Event(type)
	{
		mDelta = delta;
	}

	double getDelta() const { return mDelta; }
private:
	double mDelta;
};