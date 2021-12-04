#include "unitAddEvent.h"
#include "game.h"

UnitAddEvent::UnitAddEvent(EventType type, Unit *unit) : Event(type)
{
	mpUnit = unit;
}

UnitAddEvent::~UnitAddEvent()
{
}

Unit *UnitAddEvent::getUnit() const
{
	return mpUnit;
}
