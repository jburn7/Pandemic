#pragma once
#include "event.h"
#include "City.h"

class PlayerMoveEvent : public Event
{
public:
	PlayerMoveEvent(EventType type, const City *toCity);

	const City* getCity();

private:
	const City *mpCity;
};