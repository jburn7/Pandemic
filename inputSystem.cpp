#include "inputSystem.h"
#include "game.h"
#include "KeyEvent.h"

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
				mb = MOUSE_LEFT;
				break;
			case sf::Mouse::Button::Right:
				mb = MOUSE_RIGHT;
				break;
			default:
				mb = MOUSE_LEFT;
			}
			sf::Vector2i mousePos = sf::Mouse::getPosition(Game::getInstance()->getGraphics().mDisplay);
			gpEventSystem->fireEvent(new MouseClickEvent(MOUSE_CLICK_EVENT, Vector2D((float)mousePos.x, (float)mousePos.y), mb));
		}
		else if(e.type == sf::Event::KeyPressed)
		{
			switch(e.key.code)
			{
			case sf::Keyboard::Left:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, LEFT));
				break;
			case sf::Keyboard::Right:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, RIGHT));
				break;
			case sf::Keyboard::Up:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, UP));
				break;
			case sf::Keyboard::Down:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, DOWN));
				break;
			case sf::Keyboard::D:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, D));
				break;
			case sf::Keyboard::A:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, A));
				break;
			case sf::Keyboard::Space:
				gpEventSystem->fireEvent(new KeyPressedEvent(KEY_PRESSED_EVENT, SPACE));
				break;
			default:
				break;
			}
		}
		else if(e.type == sf::Event::KeyReleased)
		{
			switch(e.key.code)
			{
			case sf::Keyboard::Left:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, LEFT));
				break;
			case sf::Keyboard::Right:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, RIGHT));
				break;
			case sf::Keyboard::Up:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, UP));
				break;
			case sf::Keyboard::D:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, D));
				break;
			case sf::Keyboard::A:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, A));
				break;
			case sf::Keyboard::Down:
				gpEventSystem->fireEvent(new KeyReleasedEvent(KEY_RELEASED_EVENT, DOWN));
				break;
			default:
				break;
			}
		}
		else if(e.type == sf::Event::MouseWheelMoved)
		{
			const Vector2D cursorLocation = Vector2D((float)e.mouseWheel.x, (float)e.mouseWheel.y);
			switch(e.mouseWheel.delta)
			{
			case 1:
				gpEventSystem->fireEvent(new MouseWheelEvent(MOUSE_WHEEL_EVENT, MOUSE_WHEEL_UP, cursorLocation));
				break;
			case -1:
				gpEventSystem->fireEvent(new MouseWheelEvent(MOUSE_WHEEL_EVENT, MOUSE_WHEEL_DOWN, cursorLocation));
				break;
			}
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
