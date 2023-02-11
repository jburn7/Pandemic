#include "PlayerMoveEvent.h"

PlayerMoveEvent::PlayerMoveEvent(EventType type, const City* toCity) : Event(type)
{
	mpCity = toCity;
}