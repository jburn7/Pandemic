#pragma once
#include "event.h"
#include "Player.h"

class ActivePawnChangeEvent : public Event
{
public:
	ActivePawnChangeEvent(EventType type, const Player &pawn) : Event(type), mPawn(pawn) { }

	const Player &getPawn() const { return mPawn; }

private:
	const Player &mPawn;
};