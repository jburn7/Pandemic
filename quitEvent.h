#pragma once
#include "event.h"

class QuitEvent : public Event
{
public:
	QuitEvent();
	~QuitEvent();
};