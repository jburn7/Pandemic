#include "AISystem.h"
#include "AIEvents.h"
#include "EventSystem.h"

AISystem::AISystem()
{
}

AISystem::~AISystem()
{
}

void AISystem::init(int ticksPerPlayerMove)
{
	mTicksPerPlayerMove = ticksPerPlayerMove;
	mTicksCounter = 0;
	mShouldMakePlayerMove = false;
}

void AISystem::update(const Board &board)
{
	if(mTicksCounter++ >= mTicksPerPlayerMove || mShouldMakePlayerMove)
	{
		// TODO: read board and send a move event
		mShouldMakePlayerMove = false;

		/*  Really dumb AI, v0.1:
			If player's current city has a cube, decrement it
			Else move to adjacent city with cube
			Else move to random adjacent city
		*/
		City *const currentCity = board.mpActivePawn->getCurrentCity();
		if(currentCity->getNumberOfDiseaseCubes() > 0)
		{
			gpEventSystem->fireEvent(new AIPlayerCubeEvent(AI_PLAYER_CUBE_EVENT, currentCity));
		}
	}

}

void AISystem::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == AI_SHOULD_MOVE_EVENT)
	{
		mShouldMakePlayerMove = true;
	}
}
