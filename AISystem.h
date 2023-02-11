#pragma once
#include "Board.h"
#include "EventListener.h"

// The goal here is to manually poll the AI every x ticks in the game loop. We will expose the board to the AI, and then when it is polled, it will make one game move in a stateless fashion. So the AI shouldn't really hold any data and this is basically a collection of helpers. The AI will send an AI event which the board will read and then try to do - the Board should be doing correctness checks and so it should be able to handle bad/illegal inputs.

// We'll tell the AI to ready up for making a player move by sending it an event, this lets us call update() on it every tick and it can just do what it needs to do

class AISystem : public EventListener
{
public:
	AISystem();
	~AISystem();

	// Lifecycle
	void init(int ticksPerPlayerMove); 
	void update(const Board &board);

	// Events
	virtual void handleEvent(const Event& theEvent);
private:
	bool mShouldMakePlayerMove;
	int mTicksPerPlayerMove;
	int mTicksCounter;
};