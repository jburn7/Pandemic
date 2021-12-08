#pragma once
#include <string>
#include <Trackable.h>

enum EventType
{
	INVALID_EVENT_TYPE = -1,
	UNIT_ADD_EVENT,
	UNIT_REMOVE_EVENT,
	UPDATE_FPS_EVENT,
	QUIT_EVENT,
	START_EVENT,
	DECREMENT_MOVES_EVENT,
	MOUSE_CLICK_EVENT,
	PAN_CAMERA_EVENT,
	KEY_PRESSED_EVENT,
	KEY_RELEASED_EVENT,
	OUTBREAK_EVENT
};

class Event:public Trackable
{
public:
	Event( EventType type );
	virtual ~Event();

	EventType getType() const { return mType; };
protected:
	EventType mType;
};