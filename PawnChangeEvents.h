#pragma once
#include "event.h"
#include "Player.h"

class PawnChangeEvent : public Event
{
public:
	const Player& getPawn() const { return mPawn; }

protected:
	PawnChangeEvent(const Player& pawn, const EventType eventType) : Event(eventType), mPawn(pawn) {};
	const Player& mPawn;
};

class ActivePawnChangeEvent : public PawnChangeEvent
{
public:
	ActivePawnChangeEvent(const Player &pawn) : PawnChangeEvent(pawn, EventType::ACTIVE_PAWN_CHANGE_EVENT) { }
};

class SelectedPawnChangeEvent : public PawnChangeEvent
{
public:
	SelectedPawnChangeEvent(const Player& pawn) : PawnChangeEvent(pawn, EventType::SELECTED_PAWN_CHANGE_EVENT) {}
};