#include "game.h"
#include "MovementManager.h"
#include "unit.h"
#include "UnitEvents.h"
#include "EventSystem.h"
#include "Vector2Di.h"

MovementCommand::MovementCommand(const Vector2D start, const Vector2D destination, const int _milliseconds) :
	acceleration(0.f),
	decelerationDistance(0),
	destination(destination),
	distance(destination - start),
	milliseconds(std::max(_milliseconds, 1)),
	timeElapsed(0)
{
	velocity = Vector2D(0.f, 0.f);

	double averagedSpeedCoefficient = 2;
	const double idealSpeed = distance.getLength() / milliseconds;
	if(idealSpeed > 0)
	{
		const double totalIdealAccelerationDistance = idealSpeed * DEFAULT_ACCELERATION_MS;
		const double totalIdealTopSpeedSpeedDistance = distance.getLength() - totalIdealAccelerationDistance;
		const double totalAccelerationTime = 2 * DEFAULT_ACCELERATION_MS;
		const double totalTopSpeedTime = std::max((milliseconds - (int)totalAccelerationTime), 0);
		double topSpeedCoefficient = 1;
		double totalModifiedAccelerationDistance = totalAccelerationTime * idealSpeed * topSpeedCoefficient * ((totalAccelerationTime) / milliseconds);

		if(totalTopSpeedTime > 0)
		{
			const double idealModifiedTopSpeed = totalIdealTopSpeedSpeedDistance / totalTopSpeedTime;
			topSpeedCoefficient = idealModifiedTopSpeed / idealSpeed;
			totalModifiedAccelerationDistance *= topSpeedCoefficient;
			

			const double actualModifiedTopSpeed = (distance.getLength() - totalModifiedAccelerationDistance) / totalTopSpeedTime;

			averagedSpeedCoefficient = topSpeedCoefficient;
		}

		this->decelerationDistance = totalModifiedAccelerationDistance / 2;
	}

	this->topSpeedCoefficient = averagedSpeedCoefficient;
}

void MovementManager::init()
{
	msPerFrame = Game::getInstance()->getMsPerFrame();
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

void MovementManager::initiateMovement(Unit* const unit, const Vector2D destination, const int milliseconds)
{
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
		it->second.timeElapsed += timeElapsed;

		Vector2Di pos = Vector2Di((int)it->first->getPosition().getX(), (int)it->first->getPosition().getY());
		Vector2Di dest = Vector2Di((int)it->second.destination.getX(), (int)it->second.destination.getY());
		if(pos == dest)
		{
			gpEventSystem->fireEvent(new UnitArriveEvent(it->first, it->second.destination));
			mMovements.erase(it);
			continue;
		}

		Vector2D offset = it->second.destination - it->first->getPosition();
		const bool isInDecelerationRange = offset.getLength() < it->second.decelerationDistance;
		const bool isAccelerating = it->second.acceleration < 1 && !isInDecelerationRange;
		if(isAccelerating)
		{
			it->second.acceleration = it->second.timeElapsed / DEFAULT_ACCELERATION_MS;
			if(it->second.acceleration > 1)
			{
				it->second.acceleration = 1;
			}
		}
		else if(isInDecelerationRange)
		{			
			it->second.acceleration = std::max(offset.getLength() / it->second.decelerationDistance, msPerFrame / DEFAULT_ACCELERATION_MS);
		}
		
		Vector2D desiredDistanceThisFrame = it->second.distance * ((float)timeElapsed / it->second.milliseconds) * (float)it->second.topSpeedCoefficient;
		if(it->second.acceleration > 0)
		{
			desiredDistanceThisFrame *= (float)it->second.acceleration;
		}
		
		if(std::abs(offset.getX()) < std::abs(desiredDistanceThisFrame.getX()) || std::abs(offset.getY()) < std::abs(desiredDistanceThisFrame.getY()))
		{
			it->second.velocity = offset;
		}
		else
		{
			it->second.velocity = desiredDistanceThisFrame;
		}

		it->first->move(it->second.velocity);
	}
}
