#pragma once
#include "Camera.h"
#include "event.h"
#include "Vector2D.h"

class UpdateCameraEvent: public Event 
{
public:
	UpdateCameraEvent(const Camera& camera) : Event(EventType::UPDATE_CAMERA_EVENT), mCamera(camera)
	{
	}

	const Camera& getCamera() const { return mCamera; }

private:
	const Camera& mCamera;
};

class ZoomCameraEvent : public Event
{
public:
	ZoomCameraEvent(const double delta, const float zoom, const Vector2D zoomLocation, Camera& camera) : Event(EventType::ZOOM_CAMERA_EVENT), mCamera(camera)
	{
		mZoom = zoom;
		mDelta = delta;
		mZoomLocation = zoomLocation;
	}

	Camera& getCamera() const { return mCamera; }

	double getDelta() const { return mDelta; }

	float getZoom() const { return mZoom; }

	Vector2D getZoomLocation() const { return mZoomLocation; }
private:
	float mZoom;
	double mDelta;
	Vector2D mZoomLocation;

	Camera& mCamera; // Cannot make const, as the graphics system needs to adjust camera position after zooming
};