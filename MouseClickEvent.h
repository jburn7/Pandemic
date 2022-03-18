#pragma once
#include "event.h"
#include "Vector2D.h"

enum MouseButton
{
	MOUSE_LEFT,
	MOUSE_RIGHT
};

class MouseClickEvent : public Event
{
public:
	MouseClickEvent(EventType type, Vector2D pos, MouseButton mb) : Event(type)
	{
		mButton = mb;
		mPosition = pos;
	}
	~MouseClickEvent() {}

	MouseButton getButton() const { return mButton; }
	Vector2D getPosition() const { return mPosition; }
private:
	MouseButton mButton;
	Vector2D mPosition;
};

enum MouseWheel
{
	MOUSE_WHEEL_DOWN,
	MOUSE_WHEEL_UP
};

class MouseWheelEvent : public Event
{
public:
	MouseWheelEvent(EventType type, MouseWheel direction, const Vector2D cursorLocation) : Event(type)
	{
		mDirection = direction;
		mCursorLocation = cursorLocation;
	}

	MouseWheel getDirection() const { return mDirection; }

	Vector2D getCursorLocation() const { return mCursorLocation; }
private:
	MouseWheel mDirection;
	Vector2D mCursorLocation;
};