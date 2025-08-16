#pragma once
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window\Event.hpp"
#include "EventSystem.h"
#include "quitEvent.h"
#include "UnitEvents.h"
#include "startEvent.h"
#include "MouseEvents.h"

class InputSystem
{
public:
	InputSystem();

	~InputSystem();


	void cleanup();

	void init();


	void checkForInput();
protected:
	sf::Event mEvent;
	bool mMouseReleased, mKeyReleased;
};