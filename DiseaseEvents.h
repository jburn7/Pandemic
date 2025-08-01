#pragma once
#include "event.h"
#include "CityType.h"

class DiseaseEvent : public Event
{
public:
	DiseaseEvent(const EventType eventType, const CityType* const cityType) : Event(eventType), mpCityType(cityType)
	{
	}

	const CityType* const mpCityType;
};

class DecrementCubesEvent : public DiseaseEvent
{
public:
	DecrementCubesEvent(const CityType* const cityType, const int decrement = 1) : DiseaseEvent(EventType::DECREMENT_CUBES_EVENT, cityType), mDecrement(decrement)
	{
	}

	const int mDecrement;
};

class IncrementCubesEvent : public DiseaseEvent
{
public:
	IncrementCubesEvent(const CityType* const cityType, const int increment = 1) : DiseaseEvent(EventType::INCREMENT_CUBES_EVENT, cityType), mIncrement(increment)
	{
	}

	const int mIncrement;
};

class OutbreakEvent : public DiseaseEvent
{
public:
	OutbreakEvent(const CityType* const cityType) : DiseaseEvent(EventType::OUTBREAK_EVENT, cityType)
	{
	}
};