#include "MovementManager.h"
#include "unit.h"
#include "UnitEvents.h"
#include "EventSystem.h"

void MovementManager::init()
{
	gpEventSystem->addListener(EventType::UNIT_MOVE_EVENT, this);
}

void MovementManager::handleEvent(const Event& theEvent)
{
	if(theEvent.getType() == EventType::UNIT_MOVE_EVENT)
	{
		const UnitMoveEvent &ev = static_cast<const UnitMoveEvent&>(theEvent);
		initiateMovement(ev.getUnit(), ev.getDestination(), ev.getMilliseconds());
	}
}

void MovementManager::initiateMovement(Unit* const unit, const Vector2D destination, const float milliseconds)
{
	// TODO: how to handle chained movements for same unit?
	// Right now, later commands are ignored, obviously we want to change that
	auto existing = mMovements.find(unit);
	if(existing != mMovements.end())
	{
		mMovements.erase(existing);
	}

	mMovements.insert(std::pair(unit, MovementCommand(unit->getPosition(), destination, milliseconds)));
}

void MovementManager::update(double timeElapsed)
{
	for(auto it = mMovements.begin(), nextIt = it; it != mMovements.end(); it = nextIt)
	{
		nextIt++;
		const float millisecondsToTravel = it->second.milliseconds == 0 ? 1 : it->second.milliseconds;
		const Vector2D desiredVelocityThisFrame = it->second.distance * ((float)timeElapsed / millisecondsToTravel);
		// TODO: remove reference to sf:: members (either cast to int or make Vector2i wrapper)
		sf::Vector2i pos = sf::Vector2i((int)it->first->getPosition().getX(), (int)it->first->getPosition().getY());
		sf::Vector2i dest = sf::Vector2i((int)it->second.destination.getX(), (int)it->second.destination.getY());
		if(pos == dest)
		{
			// TODO: fire unit arrived event
			mMovements.erase(it);
			continue;
		}
		Vector2D offset = it->second.destination - it->first->getPosition();
		if(std::abs(offset.getX()) < std::abs(desiredVelocityThisFrame.getX()) || std::abs(offset.getY()) < std::abs(desiredVelocityThisFrame.getY()))
		{
			it->second.velocity = offset;
		}
		else
		{
			it->second.velocity = desiredVelocityThisFrame;
		}
		it->first->move(it->second.velocity);
	}
}
