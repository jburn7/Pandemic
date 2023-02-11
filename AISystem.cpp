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

	gpEventSystem->addListener(EventType::KEY_PRESSED_EVENT, this);
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
		/*  TODO: AI v.02:
			From active pawn's city, form a graph of every city that is 3 or fewer edges from city (ignore flights for now but we'll eventually consider those as one directed edge)
			Run max cost algorithm on this mini-graph, using number of cubes that a given city has to represent the cost for all of its edges (or maybe increase this cost exponentially based on number of cubes)
			Somewhere along the way in this algorithm, we'll need to account for potentially removing cubes, which would 1) lower the length of the graph and 2) adjust the cost of the city mid-traversal. So we're looking at some sort of recursive max cost algorithm?
		*/
		/*  Alternatively, we could do a different algorithm where the end city is predetermined (e.g. if the pawn has that card and it needs to be traded, we know the  pawn needs to end there, or if not then the closest city with 3 cubes that can be drawn or something would be the next end city)
			Then we can just do a single max cost algorithm rather than recursively doing one
		*/
		City *const currentCity = board.mpActivePawn->getCurrentCity();
		if(currentCity->getNumberOfDiseaseCubes() > 0)
		{
			gpEventSystem->fireEvent(new AIPlayerCubeEvent(currentCity));
			return;
		}
		std::vector<City*> neighbors = currentCity->getNeighbors();
		bool moved = false;
		for(auto &n : neighbors)
		{
			if(n->getNumberOfDiseaseCubes() > 0)
			{
				gpEventSystem->fireEvent(new AIPlayerMoveEvent(n));
				moved = true;
				return;
			}
		}
		if(!moved)
		{
			int randomCity = rand() % neighbors.size();
			gpEventSystem->fireEvent(new AIPlayerMoveEvent(neighbors[randomCity]));
			return;
		}
	}

}

void AISystem::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == EventType::KEY_PRESSED_EVENT)
	{
		const KeyPressedEvent &ev = static_cast<const KeyPressedEvent&>(theEvent);
		switch(ev.getKey())
		{
		case Key::SPACE:
			mShouldMakePlayerMove = true;
			break;
		default:
			break;
		}
	}
}
