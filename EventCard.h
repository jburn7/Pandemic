#pragma once
#include "unit.h"

enum class SpecialEvent
{
	ONE_QUIET_NIGHT,
	RESILIENT_POP,
	AIRLIFT,
	GOVERNMENT_GRANT,
	FORECAST
};

class EventCard : public Unit
{
public:
	EventCard(SpecialEvent eid)
	{
		eventID = eid;
	}

	~EventCard() {}

	virtual void play()
	{

	}

private:
	SpecialEvent eventID; //map of cityID to city maintained by Board
};