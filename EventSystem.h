#pragma once
#include <map>
#include <vector>
#include <Trackable.h>

class Event;
class EventListener;
enum class EventType;

using namespace std;

class EventSystem :public Trackable
{
public:
	EventSystem();
	~EventSystem();

	void fireEvent(const Event *theEvent);
	void addListener(EventType type, EventListener* pListener);
	//void fireAll();
	void removeListener(EventType type, EventListener* pListener);
	void removeListenerFromAllEvents(EventListener* pListener);
	void dispatchAllEvents();

private:
	static EventSystem* msInstance;
	multimap< EventType, EventListener* > mListenerMap;

	std::vector<const Event*> mQueue, mSecondaryQueue; //events that are fired inside of handleEvent() MUST be added to mSecondaryQueue
	bool mDispatching; //set to true while inside the dispatch loop. if true, add Events to secondary queue
};

extern EventSystem* gpEventSystem;