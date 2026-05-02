#pragma once
#include "Camera.h"
#include "event.h"
#include "EventSystem.h"
#include "EventListener.h"
#include "Moveable.h"
#include "Vector2D.h"

class CameraManager : public EventListener
{
public:
	CameraManager();

	void init(const Vector2D bounds, const float panSpeed, const float panAcceleration, const float zoomAmount, const float maxZoom, const float minZoom);

	// Read pan/zoom states and tell graphics system to react accordingly
	void update();

	//event functions
	virtual void handleEvent(const Event& theEvent);

	// Getters
	Camera& getCamera();

private:
	bool mCameraPanLeft, mCameraPanRight, mCameraPanUp, mCameraPanDown; // If we want to get fancy we could move these into two enums or something

	float mPanSpeed, mMaxPanSpeed, mPanAcceleration;
	float mZoomPerTick, mMaxZoom, mMinZoom;
	Vector2D mBounds; // Abs val of how far panning can stray from origin

	Camera mCamera;
};