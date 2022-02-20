#pragma once
#include "Trackable.h"
#include "event.h"
#include "EventSystem.h"
#include "EventListener.h"
#include "Vector2D.h"

class CameraManager : public EventListener
{
public:
	void init(const Vector2D bounds, const float panSpeed, const float panAcceleration);

	// Read pan/zoom states and tell graphics system to react accordingly
	void update();

	//event functions
	virtual void handleEvent(const Event& theEvent);

	// Getters
	const Vector2D getOffset();

private:
	// TODO: add zoom in/out for quicker navigation
	bool mCameraPanLeft, mCameraPanRight, mCameraPanUp, mCameraPanDown; // If we want to get fancy we could move these into two enums or something

	float mPanSpeed, mMaxPanSpeed, mPanAcceleration;
	Vector2D mBounds; // Abs val of how far panning can stray from origin
	Vector2D mCenter;
};