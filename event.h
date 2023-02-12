#pragma once
#include <string>
#include <Trackable.h>

enum class EventType
{
	INVALID_EVENT_TYPE = -1,
	UNIT_ADD_EVENT,
	UNIT_REMOVE_EVENT,
	UNIT_MOVE_EVENT,
	UPDATE_FPS_EVENT,
	QUIT_EVENT,
	START_EVENT,
	DECREMENT_MOVES_EVENT,
	MOUSE_CLICK_EVENT,
	MOUSE_WHEEL_EVENT,
	PAN_CAMERA_EVENT,
	ZOOM_CAMERA_EVENT,
	KEY_PRESSED_EVENT,
	KEY_RELEASED_EVENT,
	PLAYER_MOVE_EVENT,
	AI_SHOULD_MOVE_EVENT,
	AI_PLAYER_MOVE_EVENT,
	AI_PLAYER_CUBE_EVENT,
	OUTBREAK_EVENT,
	EPIDEMIC_EVENT,
	ACTIVE_PAWN_CHANGE_EVENT,
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