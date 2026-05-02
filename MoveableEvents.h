#pragma once
#include "event.h"
#include "Moveable.h"

class MoveableEvent : public Event
{
public:
	Moveable* getMoveable() const { return mpMoveable; }

protected:
	MoveableEvent(EventType type, Moveable* const moveable) : Event(type), mpMoveable(moveable) {};
	Moveable* const mpMoveable;
};

class MoveableArriveEvent : public MoveableEvent
{
public:
	MoveableArriveEvent(Moveable* const moveable, const Vector2D destination) :
		MoveableEvent(EventType::MOVEABLE_ARRIVE_EVENT, moveable),
		mDestination(destination)
	{
	};

	const Vector2D& getDestination() const { return mDestination; }

private:
	const Vector2D mDestination;
};

class MoveableMoveEvent : public MoveableEvent
{
public:
	MoveableMoveEvent(Moveable* const moveable, const Vector2D destination, const int milliseconds) :
		MoveableEvent(EventType::MOVEABLE_MOVE_EVENT, moveable),
		mDestination(destination),
		mMilliseconds(milliseconds)
	{
	};

	const Vector2D& getDestination() const { return mDestination; }
	const int getMilliseconds() const { return mMilliseconds; }

protected:
	const Vector2D mDestination;
	const int mMilliseconds;
};

class CameraMoveEvent : public MoveableMoveEvent
{
public:
	CameraMoveEvent(const Vector2D destination, const int milliseconds) :
		MoveableMoveEvent(&Game::getInstance()->getCameraManager().getCamera(), destination, milliseconds),
		mDestination(destination),
		mMilliseconds(milliseconds)
	{
	};

	const Vector2D& getDestination() const { return mDestination; }
	const int getMilliseconds() const { return mMilliseconds; }

private:
	const Vector2D mDestination;
	const int mMilliseconds;
};