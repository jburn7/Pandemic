#pragma once
#include "Card.h"

enum SpecialEvent
{
	ONE_QUIET_NIGHT,
	RESILIENT_POP,
	AIRLIFT,
	GOVERNMENT_GRANT,
	FORECAST
};

class EventCard : public Card
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