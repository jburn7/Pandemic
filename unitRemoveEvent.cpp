#include "unitRemoveEvent.h"

UnitRemoveEvent::UnitRemoveEvent(EventType type, Unit *u) : Event(type)
{
	mpToRemove = u;
}

UnitRemoveEvent::~UnitRemoveEvent()
{
}
