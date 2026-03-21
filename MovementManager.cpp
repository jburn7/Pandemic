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
	pixelsLost(0),
	milliseconds(std::max(_milliseconds, 1)),
	timeBeganDecelerating(0),
	timeElapsed(0),
	idealConstantPixelsPerMs(distance.getLength() / milliseconds),
	estimatedLostPixelsPerMs(idealConstantPixelsPerMs* (2 * DEFAULT_ACCELERATION_MS / milliseconds))
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

			averagedSpeedCoefficient = topSpeedCoefficient;// actualModifiedTopSpeed / idealModifiedTopSpeed;
		}

		this->decelerationDistance = totalModifiedAccelerationDistance / 2;
	}

	this->topSpeedCoefficient = averagedSpeedCoefficient;
}

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
			std::cout << "actual distance lost: " << it->second.pixelsLost << std::endl;
			std::cout << "estimated distance lost: " << it->second.estimatedLostPixelsPerMs * it->second.milliseconds << std::endl;
			std::cout << "time offset: " << it->second.timeElapsed - it->second.milliseconds << std::endl;
			gpEventSystem->fireEvent(new UnitArriveEvent(it->first, it->second.destination));
			mMovements.erase(it);
			continue;
		}

		Vector2D offset = it->second.destination - it->first->getPosition();
		const bool isInDecelerationRange = offset.getLength() < it->second.decelerationDistance;
		const bool isAccelerating = it->second.acceleration < 1 && !isInDecelerationRange;
		const double accelerationLastFrame = it->second.acceleration;
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
			it->second.acceleration = std::max(offset.getLength() / it->second.decelerationDistance, 17 / DEFAULT_ACCELERATION_MS);
		}

		/*
		* TODO: there is some coefficient that I'm missing here
		* when it is 1 and milliseconds <= 2 * accel window, then it works
		* but as the gap between milliseconds and accel window grows, the coefficient changes in order to get the unit there on time
		* 
		* coincedentally also works when milliseconds is 2000??
		* 300 / 600 * x = 2
		* 300 / 1500 * x = 1.25
		* 300 / 3000 * x = 1.06
		* 300 / 6000 * x = 0.95
		* 300 / 9000 * x = 0.86
		*/
		//const double mysteryCoefficient = 1.5;
		// coef = (percentage of time spent accelerating) * (estimated loss / ideal constant) + (percentage of time not spent accelerating) * ???
		const double percentageOfAccelerationTime = (DEFAULT_ACCELERATION_MS * 2 / it->second.milliseconds);
		const double distanceLostToIdealConstantSpeedRatio = it->second.estimatedLostPixelsPerMs / it->second.idealConstantPixelsPerMs;

		const double totalLostPixelsThisFrame = it->second.estimatedLostPixelsPerMs * timeElapsed;
		

		const double percentageOfTopSpeedTime = 1 - percentageOfAccelerationTime;
		//double topSpeedCoefficient = (percentageOfTopSpeedTime) + percentageOfAccelerationTime * distanceLostToIdealConstantSpeedRatio;
		Vector2D desiredDistanceThisFrameBeforeSpeedModifier = it->second.distance * ((float)timeElapsed / it->second.milliseconds);

		const double lengthThisFrame = desiredDistanceThisFrameBeforeSpeedModifier.getLength();
		const double ratioLostThisFrame = totalLostPixelsThisFrame * (it->second.acceleration / (1 + percentageOfAccelerationTime)) / lengthThisFrame;




		//std::cout << "distance: " << it->second.distance.getLength() << std::endl;
		//std::cout << "idealSpeed: " << idealSpeed << std::endl;
		//std::cout << "totalIdealAccelerationDistance: " << totalIdealAccelerationDistance << std::endl;
		//std::cout << "totalIdealTopSpeedSpeedDistance: " << totalIdealTopSpeedSpeedDistance << std::endl;
		//std::cout << "idealModifiedTopSpeed: " << idealModifiedTopSpeed << std::endl;
		//std::cout << "topSpeedCoefficient: " << topSpeedCoefficient << std::endl;
		//std::cout << "totalModifiedAccelerationDistance: " << totalModifiedAccelerationDistance << std::endl;
		//std::cout << "actualModifiedTopSpeed: " << actualModifiedTopSpeed << std::endl;
		//std::cout << "averagedSpeedCoefficient: " << averagedSpeedCoefficient << std::endl;
		std::cout << "acceleration: " << it->second.acceleration<< std::endl;
		std::cout << "topSpeedCoefficient: " << it->second.topSpeedCoefficient << std::endl;


		Vector2D desiredDistanceThisFrame = desiredDistanceThisFrameBeforeSpeedModifier * it->second.topSpeedCoefficient;
		if(it->second.acceleration == 1)
		{
			//desiredDistanceThisFrame *= it->second.topSpeedCoefficient;
		}
		if(it->second.acceleration > 0)
		{
			desiredDistanceThisFrame *= (float)it->second.acceleration;
		}
		
		if(std::abs(offset.getX()) < std::abs(desiredDistanceThisFrame.getX()) || std::abs(offset.getY()) < std::abs(desiredDistanceThisFrame.getY()))
		{
			it->second.velocity = offset;
			std::cout << std::endl;
		}
		else
		{
			it->second.velocity = desiredDistanceThisFrame;
			//std::cout << "ratioLostThisFrame: " << ratioLostThisFrame << std::endl;
			std::cout << std::endl;
		}

		if(isAccelerating || isInDecelerationRange)
		{
			it->second.pixelsLost += desiredDistanceThisFrameBeforeSpeedModifier.getLength() / timeElapsed - it->second.idealConstantPixelsPerMs;
		}

		it->first->move(it->second.velocity);
	}
}
