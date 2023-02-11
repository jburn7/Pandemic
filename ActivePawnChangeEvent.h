#pragma once
#include "event.h"
#include "Player.h"

class ActivePawnChangeEvent : public Event
{
public:
	ActivePawnChangeEvent(const Player &pawn) : Event(EventType::ACTIVE_PAWN_CHANGE_EVENT), mPawn(pawn) { }

	const Player &getPawn() const { return mPawn; }

private:
	const Player &mPawn;
};