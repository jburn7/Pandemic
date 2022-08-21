#pragma once
#include "event.h"

// TODO: Remove
class QuitEvent : public Event
{
public:
	QuitEvent();
	~QuitEvent();
};