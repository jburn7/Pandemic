#include "AISystem.h"
#include "AIEvents.h"
#include "EventSystem.h"
#include "KeyEvent.h"

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

	gpEventSystem->addListener(KEY_PRESSED_EVENT, this);
}

void AISystem::update(const Board &board)
{
	if(mTicksCounter++ >= mTicksPerPlayerMove || mShouldMakePlayerMove)
	{
		mShouldMakePlayerMove = false;
		mTicksCounter = 0;

		/*  Really dumb AI, v0.1:
			If player's current city has a cube, decrement it
			Else move to adjacent city with cube
			Else move to random adjacent city
		*/
		City *const currentCity = board.mpActivePawn->getCurrentCity();
		if(currentCity->getNumberOfDiseaseCubes() > 0)
		{
			gpEventSystem->fireEvent(new AIPlayerCubeEvent(AI_PLAYER_CUBE_EVENT, currentCity));
			return;
		}
		std::vector<City*> neighbors = currentCity->getNeighbors();
		bool moved = false;
		for(auto &n : neighbors)
		{
			if(n->getNumberOfDiseaseCubes() > 0)
			{
				gpEventSystem->fireEvent(new AIPlayerMoveEvent(AI_PLAYER_MOVE_EVENT, n));
				moved = true;
				return;
			}
		}
		if(!moved)
		{
			int randomCity = rand() % neighbors.size();
			gpEventSystem->fireEvent(new AIPlayerMoveEvent(AI_PLAYER_MOVE_EVENT, neighbors[randomCity]));
			return;
		}
	}

}

void AISystem::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == KEY_PRESSED_EVENT)
	{
		const KeyPressedEvent &ev = static_cast<const KeyPressedEvent&>(theEvent);
		switch(ev.getKey())
		{
		case SPACE:
			mShouldMakePlayerMove = true;
			break;
		default:
			break;
		}
	}
}
