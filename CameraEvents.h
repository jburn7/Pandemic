#pragma once
#include "event.h"
#include "Vector2D.h"

class PlaceCameraEvent : public Event
{
public:
	PlaceCameraEvent(const Vector2D position) : Event(EventType::PLACE_CAMERA_EVENT), mPosition(position)
	{
	}

	const Vector2D& getPosition() const { return mPosition; }

private:
	const Vector2D mPosition;
};

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