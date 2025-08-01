#pragma once
#include "event.h"
#include "CityType.h"

class OutbreakEvent : public Event
{
public:
	OutbreakEvent(const CityType* const cityType) : Event(EventType::OUTBREAK_EVENT), mpCityType(cityType) {}
private:
	const CityType* const mpCityType;
};