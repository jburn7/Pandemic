#pragma once
#include "event.h"
#include "EventSystem.h"
#include "EventListener.h"
#include "Moveable.h"
#include "Vector2D.h"

class Camera : public Moveable, public EventListener
{
public:
	Camera();

	void init(const Vector2D bounds, const float panSpeed, const float panAcceleration, const float zoomAmount, const float maxZoom, const float minZoom);

	// Read pan/zoom states and tell graphics system to react accordingly
	void update();

	// Overrides
	virtual void setPosition(Vector2D pos) override;

	//event functions
	virtual void handleEvent(const Event& theEvent);

	// Setters
	void setZoom(const float zoom, const Vector2D& position);

	// Getters
	Camera& getCamera();

	const float getCurrentZoom() const;

private:
	bool mCameraPanLeft, mCameraPanRight, mCameraPanUp, mCameraPanDown; // If we want to get fancy we could move these into two enums or something

	float mPanSpeed, mMaxPanSpeed, mPanAcceleration;
	float mZoomPerTick, mMaxZoom, mMinZoom;
	float mCurrentZoom;
	Vector2D mBounds; // Abs val of how far panning can stray from origin
};