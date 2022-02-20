#pragma once
#include "event.h"
#include "City.h"
#include "CityCard.h"

class AIPlayerMoveEvent : public Event
{
public:
	AIPlayerMoveEvent(EventType type, const City *targetCity, const CityCard *cardUsed) : Event(type)
	{
		mpTargetCity = targetCity;
		mpCardUsed = cardUsed;
	}

private:
	const City *mpTargetCity;
	const CityCard *mpCardUsed;
};

class AIPlayerCubeEvent : public Event
{
public:
	AIPlayerCubeEvent(EventType type, City* targetCity) : Event(type), mpTargetCity(targetCity)
	{
	}

	City *getCity() const { return mpTargetCity; }

private:
	City* const mpTargetCity;
};