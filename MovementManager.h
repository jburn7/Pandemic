#pragma once
#include "EventListener.h"
#include "Moveable.h"
#include "Vector2D.h"
#include <map>

const double DEFAULT_ACCELERATION_MS = 300;

class MovementCommand
{
	friend class MovementManager;
private:
	MovementCommand(const Vector2D start, const Vector2D destination, const int _milliseconds);
	
	double acceleration; // 0 to 1, acts as coefficient on velocity for beginning and ending frames, velocity is increased to make up for lost time
	double decelerationDistance;
	const Vector2D destination;
	const Vector2D distance;
	const int milliseconds;
	double timeElapsed;
	double topSpeedCoefficient;
	Vector2D velocity;
};

/*
* This class will be responsible for providing "animated" movements of all Moveables (mainly units, also Camera).
* I could have just made units know where they want to go/how fast to go there
* on their own, but I like the idea of a centralized system that I can tell "move X to Y,Z in S seconds". When that movement is done, this will fire an event
* saying so. And then units can listen to that event and decide what to do once they know that they've arrived where they were going.
* The biggest drawback here might be that I will need to static cast the unit if another system wants to listen to that Unit's arrival, e.g. if the Board wants to know when a card makes its way to the discard pile so that it can keep the game state in sync with the animation state or something. But that can probably be remedied by an internal list that the Board keeps track of and then it matches the Unit pointer in the event to the items in that list to determine what exactly it was that finished moving.
* Or maybe I'm prematurely optimizing and won't need to worry about that use case.
* 
* Conceptually, this isn't much different from an AI walking system in a more traditional game.
*/

class MovementManager : public EventListener
{
public:
	void init();

	virtual void handleEvent(const Event& theEvent);

	void initiateMovement(Moveable* const moveable, const Vector2D destination, const int milliseconds);

	void update(double timeElapsed);

private:
	int msPerFrame;

	std::map<Moveable* const, MovementCommand> mMovements;
};