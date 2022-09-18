#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

EventSystem* gpEventSystem = NULL;

EventSystem::EventSystem()
{
	mDispatching = false;
}

EventSystem::~EventSystem()
{
	for(auto a : mQueue)
	{
		delete a;
	}
	for(auto b : mSecondaryQueue)
	{
		delete b;
	}

	mQueue.clear();
	mSecondaryQueue.clear();
}

void EventSystem::addListener(EventType type, EventListener* pListener)
{
	mListenerMap.insert(std::pair< EventType, EventListener* >(type, pListener));
}

//void EventSystem::fireAll()
//{
//	for(unsigned int i = 0; i < mQueue.size(); i++)
//	{
//		dispatchAllEvents(*mQueue[i]);
//	}
//
//	mQueue.clear();
//}

void EventSystem::removeListener(EventType type, EventListener *pListener)
{
	std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::iterator> ret;

	ret = mListenerMap.equal_range(type);
	std::multimap<EventType, EventListener*>::iterator iter;

	for(iter = ret.first; iter != ret.second; ++iter)
	{
		if(iter->second == pListener)
		{
			mListenerMap.erase(iter);
			break;//to prevent using invalidated iterator
		}
	}
}

void EventSystem::removeListenerFromAllEvents(EventListener* pListener)
{
	std::multimap<EventType, EventListener*>::iterator iter;

	bool allTheWayThrough = false;

	while(!allTheWayThrough)
	{
		allTheWayThrough = true;
		for(iter = mListenerMap.begin(); iter != mListenerMap.end(); ++iter)
		{
			if(iter->second == pListener)
			{
				mListenerMap.erase(iter);
				allTheWayThrough = false; //didn't make it the whole way through
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::fireEvent(const Event* theEvent)
{
	//dispatchAllEvents(theEvent);
	if(!mDispatching)
	{
		mQueue.push_back(theEvent);
	}
	else
	{
		mSecondaryQueue.push_back(theEvent);
	}
}

void EventSystem::dispatchAllEvents()
{
	for(auto a : mSecondaryQueue)
	{
		mQueue.push_back(a);
	}
	mSecondaryQueue.clear();
	std::pair<std::multimap<EventType, EventListener*>::iterator, std::multimap<EventType, EventListener*>::const_iterator> ret;
	std::multimap<EventType, EventListener*>::const_iterator iter;
	mDispatching = true;
	for(const Event* theEvent : mQueue)
	{
		ret = mListenerMap.equal_range(theEvent->getType());
		for(iter = ret.first; iter != ret.second; ++iter)
		{
			iter->second->handleEvent(*theEvent);
		}
		delete theEvent;
	}
	mDispatching = false;
	mQueue.clear();
}