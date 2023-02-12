#pragma once
#include "event.h"
#include "unit.h"

class UnitEvent : public Event
{
public:
	Unit* getUnit() const { return mpUnit; }

protected:
	UnitEvent(EventType type, Unit* const unit) : Event(type), mpUnit(unit) { };
	Unit* const mpUnit;
};

class UnitAddEvent : public UnitEvent
{
public:
	UnitAddEvent(Unit* unit) : UnitEvent(EventType::UNIT_ADD_EVENT, unit) {};
};

class UnitRemoveEvent : public UnitEvent
{
public:
	UnitRemoveEvent(Unit* const unit) : UnitEvent(EventType::UNIT_REMOVE_EVENT, unit) {};
};

class UnitMoveEvent : public UnitEvent
{
public:
	UnitMoveEvent(Unit* const unit, const Vector2D destination, const float milliseconds) :
		UnitEvent(EventType::UNIT_MOVE_EVENT, unit),
		mDestination(destination),
		mMilliseconds(milliseconds)
	{};

	const Vector2D &getDestination() const { return mDestination; }
	const float &getMilliseconds() const { return mMilliseconds; }

private:
	const Vector2D mDestination;
	const float mMilliseconds;
};