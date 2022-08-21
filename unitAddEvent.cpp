#include "unitAddEvent.h"
#include "game.h"

UnitAddEvent::UnitAddEvent(Unit *unit) : Event(EventType::UNIT_ADD_EVENT)
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
