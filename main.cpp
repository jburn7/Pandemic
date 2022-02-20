/*author: Jordan Burnett
	This program will be a working digital copy of the board game Pandemic by Z-MAN games. It is intended only for personal use
	and educational purposes.

	****Documentation****
	
	City: holds a list of neighboring cities, number of disease cubes

	Player: holds list of Player Cards, number of moves remaining, city location

	Player Card: holds information such as city

	Event Card: holds information about what event this is

	Infection Card: same as player card

	Board: stores a list of cities and list of players, and lists of cards (player draw, player discard, infection draw, infection discard)
		additionally keeps track of total number of disease cubes allocated to cities, number of outbreaks, infection rate
*/

#include "game.h"
#include "MemoryTracker.h"

int main()
{
	srand(time(NULL));
	gpEventSystem = new EventSystem;
	Game::initInstance();
	Game::getInstance()->init("data/jsonData.json");
	Game::getInstance()->loop();
	Game::getInstance()->cleanup();

	Game::cleanupInstance();

	delete gpEventSystem;

	gMemoryTracker.reportAllocations(std::cout);
	system("pause");

	return 0;
}