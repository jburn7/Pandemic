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