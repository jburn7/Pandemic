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

void MovementManager::initiateMovement(Unit* const unit, const Vector2D destination, const double milliseconds)
{
	// TODO: how to handle chained movements for same unit?
	mMovements.insert(std::pair(unit, MovementCommand(unit->getPosition(), destination, milliseconds)));
}

void MovementManager::update(double timeElapsed)
{
	for(auto it = mMovements.begin(), nextIt = it; it != mMovements.end(); it = nextIt)
	{
		nextIt++;
		it->first->move(it->second.velocity); // TODO: utilize timeElapsed to tie speed to current FPS
		// TODO: remove reference to sf:: members (either cast to int or make Vector2i wrapper)
		sf::Vector2i pos = sf::Vector2i(it->first->getPosition().getX(), it->first->getPosition().getY());
		sf::Vector2i dest = sf::Vector2i(it->second.destination.getX(), it->second.destination.getY());
		Vector2D offset = it->second.destination - it->first->getPosition();
		if(offset.getX() < it->second.velocity.getX() || offset.getY() < it->second.velocity.getY())
		{
			it->second.velocity = offset;
		}
		else if(pos == dest)
		{
			// TODO: fire unit arrived event
			mMovements.erase(it);
		}
	}
}
