#include "inputSystem.h"
#include "game.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	cleanup();
}

void InputSystem::cleanup()
{
}

void InputSystem::init()
{
}

void InputSystem::checkForInput()
{
	sf::Event e;
	while(Game::getInstance()->getGraphics().mDisplay.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
		{
			gpEventSystem->fireEvent(new QuitEvent(QUIT_EVENT));
		}
		else if(e.type == sf::Event::MouseButtonPressed)
		{
			MouseButton mb;
			switch(e.mouseButton.button)
			{
			case sf::Mouse::Button::Left:
				mb = LEFT;
				break;
			case sf::Mouse::Button::Right:
				mb = RIGHT;
				break;
			default:
				mb = LEFT;
			}
			sf::Vector2i mousePos = sf::Mouse::getPosition(Game::getInstance()->getGraphics().mDisplay);
			gpEventSystem->fireEvent(new MouseClickEvent(MOUSE_CLICK_EVENT, Vector2D((float)mousePos.x, (float)mousePos.y), mb));
		}
	}
	if(Game::getInstance()->getGamestate() == PLAYING)
	{
		/*
		if mouse is clicked
			fire a mouse click event containing coords of click
				board will listen to this, and it will resolve click
					in board: if click landed on city adjacent to active pawn, move that pawn
						if click landed on same city of active pawn and no active cards, reduce that city's disease cubes
						if click landed on a card, set that card to active and resolve that in any future clicks (eg set card to active, then click on own city to perform
							charter flight action, or do trades if player clicks in card area of another player, etc)

		if wasd is pressed
			fire move camera event in direction of wasd press. game will listen to this one and adjust the view accordingly
		if wasd is released
			fire stop camera move event as above
		*/
		
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		gpEventSystem->fireEvent(new QuitEvent(QUIT_EVENT));
	}
}
