#pragma once
#include "event.h"
#include "City.h"
#include "CityCard.h"

class AIPlayerMoveEvent : public Event
{
public:
	AIPlayerMoveEvent(EventType type, City* const targetCity, CityCard* const cardUsed = nullptr) : Event(type), mpTargetCity(targetCity), mpCardUsed(cardUsed)
	{
	}

	City *getCity() const { return mpTargetCity; }

private:
	City* const mpTargetCity;
	CityCard* const mpCardUsed;
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