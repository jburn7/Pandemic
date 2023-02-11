#pragma once
#include "event.h"
#include "City.h"
#include "CityCard.h"

class AIPlayerMoveEvent : public Event
{
public:
	AIPlayerMoveEvent(City* const targetCity, CityCard* const cardUsed = nullptr) : Event(EventType::AI_PLAYER_MOVE_EVENT), mpTargetCity(targetCity), mpCardUsed(cardUsed)
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
	AIPlayerCubeEvent(City* targetCity) : Event(EventType::AI_PLAYER_CUBE_EVENT), mpTargetCity(targetCity)
	{
	}

	City *getCity() const { return mpTargetCity; }

private:
	City* const mpTargetCity;
};