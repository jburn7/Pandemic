#pragma once
#include "event.h"
#include "Vector2D.h"

class PanCameraEvent : public Event
{
public:
	PanCameraEvent(const Vector2D delta) : Event(EventType::PAN_CAMERA_EVENT)
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
	ZoomCameraEvent(const double delta, const Vector2D zoomLocation) : Event(EventType::ZOOM_CAMERA_EVENT)
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