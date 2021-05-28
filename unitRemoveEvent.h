#pragma once
#include "Event.h"
#include "unit.h"

class UnitRemoveEvent : public Event
{
public:
	UnitRemoveEvent(EventType type, Unit *u);
	~UnitRemoveEvent();

	Unit *getToRemove() const { return mpToRemove; }
protected:
	Unit *mpToRemove;
};