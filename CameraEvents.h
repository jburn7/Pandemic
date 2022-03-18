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
	ZoomCameraEvent(EventType type, const double delta, const Vector2D zoomLocation) : Event(type)
	{
		mDelta = delta;
		mZoomLocation = zoomLocation;
	}

	double getDelta() const { return mDelta; }

	Vector2D getZoomLocation() const { return mZoomLocation; }
private:
	double mDelta;
	Vector2D mZoomLocation;
};