#pragma once
#include "Trackable.h"
#include "event.h"
#include "EventSystem.h"
#include "EventListener.h"
#include "Vector2D.h"

class CameraManager : public EventListener
{
	// TODO: move board's pan-handling (ha) to here, then fire the actual pan event if it is valid based on board's pannable size
	// TODO: pin discard piles and hands to relative place in panned view rather than to actual board space itself (eg have board update those positions when it receives a pan cam event)
public:
	void init(const Vector2D bounds, const float panSpeed);

	// Read pan/zoom states and tell graphics system to react accordingly
	void update();

	//event functions
	virtual void handleEvent(const Event& theEvent);

private:
	bool mCameraPanLeft, mCameraPanRight, mCameraPanUp, mCameraPanDown; // If we want to get fancy we could move these into two enums or something

	float mPanSpeed;
	Vector2D mBounds; // Abs val of how far panning can stray from origin
	Vector2D mCenter;
};