#pragma once
#include "Event.h"
#include "unit.h"

class UnitAddEvent : public Event
{
public:
	UnitAddEvent(Unit *unit);
	~UnitAddEvent();

	Unit *getUnit() const;
protected:
	Unit *mpUnit;
};