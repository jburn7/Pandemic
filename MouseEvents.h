#pragma once
#include "event.h"
#include "Vector2D.h"

enum class MouseButton
{
	LEFT,
	RIGHT
};

class MouseClickEvent : public Event
{
public:
	MouseClickEvent(Vector2D pos, MouseButton mb) : Event(EventType::MOUSE_CLICK_EVENT)
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

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(Vector2D pos) : Event(EventType::MOUSE_MOVE_EVENT)
	{
		mPosition = pos;
	}
	~MouseMoveEvent() {}

	Vector2D getPosition() const { return mPosition; }
private:
	Vector2D mPosition;
};

enum class MouseWheel
{
	DOWN,
	UP
};

class MouseWheelEvent : public Event
{
public:
	MouseWheelEvent(MouseWheel direction, const Vector2D cursorLocation) : Event(EventType::MOUSE_WHEEL_EVENT)
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