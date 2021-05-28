#include "Board.h"
#include "game.h"

#include "SFML\Graphics.hpp"

Board::Board()
{
}

Board::~Board()
{
	cleanup();
}

void Board::init(int numPlayers)
{
	//load cities
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	rapidjson::Value &c = doc["cities"];
	//build array of neighbors for each city
	std::vector<std::vector<int>> neighborMap;
	int numTypes = 0; // determine number of disease types based on highest city type seen in data loading
	for(auto &v : c.GetArray())
	{
		int type = v["type"].GetInt();
		if(type > numTypes)
		{
			numTypes = type;
		}
		City *city = new City(v["name"].GetString(), type, Vector2D(v["posX"].GetFloat(), v["posY"].GetFloat()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city.png"))); //deletes will be called in UnitManager destructor and Unit destructor respectively
		city->setScale(1, 1);
		mCities.push_back(city);
		neighborMap.push_back(std::vector<int>());
		for(auto &n : v["neighbors"].GetArray())
		{
			neighborMap.back().push_back(n.GetInt());
		}
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, city));
	}

	// Init disease type info
	for(unsigned int i = 0; i <= numTypes; i++)
	{
		mDiseaseStages.push_back(Spreading);
		mDiseaseCubesRemainingByType.push_back(0);
	}

	//now that all cities neighbors are loaded, find their neighbors and set the pointers
	//neighbor corresponds to position in JSON array, and thus position in mCities
	for(unsigned int i = 0; i < mCities.size(); i++)
	{
		mCities[i]->loadNeighbors(mCities, neighborMap[i]);
	}

	mPlayerDrawLocation = Vector2D(doc["game"]["playerDrawLocation"].GetArray()[0].GetInt(), doc["game"]["playerDrawLocation"].GetArray()[1].GetInt());

	//generate all cards and then shuffle
	for(unsigned int i = 0; i < mCities.size(); i++)
	{
		Sprite *cityCardBackground = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png"));
		//for each city, generate a card by placing text onto the city card sprite

		PlayerCard *pc = new PlayerCard(mPlayerDrawLocation, cityCardBackground, mCities[i]);
		playerDraw.push_back(pc);
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, pc));
	}
	//std::random_shuffle(playerDraw.begin(), playerDraw.end());

	//load in players. they should all start at Atlanta according to rules, which should always be first city in array
	for(int i = 0; i < numPlayers; i++)
	{
		//TODO: set players to different colors
		Player *p = new Player(mCities[0], std::vector<PlayerCard*>(), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pawn"]["pawnSprite"].GetString())));
		mPlayers.push_back(p);
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, p));
	}

	mpActiveCard = nullptr;
	mActivePawnIndex = 0;
	mpActivePawn = mPlayers[mActivePawnIndex];

	//now deal player cards to players
	int initialHandSize;
	switch(mPlayers.size())
	{
	case 1:
		initialHandSize = 3; //debug value, as game doesn't allow for just one player
		break;
	case 2:
		initialHandSize = 4;
		break;
	case 3:
		initialHandSize = 3;
		break;
	case 4:
		initialHandSize = 2;
		break;
	default:
		initialHandSize = 0;
		break;
	}
	for(unsigned int i = 0; i < mPlayers.size(); i++)
	{
		for(unsigned int j = 0; j < initialHandSize; j++)
		{
			dealTopPlayerCard(mpActivePawn);
		}
	}

	//TODO: dole out disease cubes based on initNumCitiesCubes in json data
	//for i where i = initNumCitiesCubes[j], give random city j + 1 cubes

	mMaxMovesPerTurn = mMovesRemaining = doc["game"]["movesPerTurn"].GetInt();
	// TODO: store this in json array[2] and find a way to make it more intuitive (based on screen width and card size rather than hardcoded)
	mPlayerDiscardLocation = Vector2D(doc["game"]["playerDiscardLocation"].GetArray()[0].GetInt(), doc["game"]["playerDiscardLocation"].GetArray()[1].GetInt());
	mNumPlayerCardsToDraw = doc["game"]["numPlayerCardsToDraw"].GetInt();

	gpEventSystem->addListener(DECREMENT_MOVES_EVENT, this);
	gpEventSystem->addListener(MOUSE_CLICK_EVENT, this);
}

void Board::cleanup()
{
	for(auto &v : mCities)
	{
		v = nullptr;
	}
	mCities.clear();

	for(auto &v : mPlayers)
	{
		v = nullptr;
	}
	mPlayers.clear();
}

void Board::dealTopPlayerCard(Player *player)
{
	if(playerDraw.size() > 0)
	{
		PlayerCard* pc = *playerDraw.begin();
		player->dealCard(pc);
		playerDraw.erase(playerDraw.begin());
	}
	else
	{
		//TODO: end game as loss
		std::cout << "TODO: you lost\n";
	}
}

bool Board::decrementDiseaseCubes(City *city)
{
	bool didDecrement = city->decrementDiseaseCubes(1);
	if(didDecrement)
	{
		if(mDiseaseStages[city->getType()] == Cured)
		{
			mDiseaseStages[city->getType()] = Eradicated;
		}
	}
	return didDecrement;
}

void Board::discardPlayerCard(Player *player, PlayerCard *card)
{
	player->discardCard(card);
	card->setPosition(mPlayerDiscardLocation);
	playerDiscard.push_back(card);
}

void Board::endTurn()
{
	/*
		Player draws two player cards
			Resolve epidemic(s)
		Player draws X infection cards
			For each, place cube, outbreaks should be internally resolved
		Change active player

	*/

	for(unsigned int i = 0; i < mNumPlayerCardsToDraw; i++)
	{
		dealTopPlayerCard(mpActivePawn);
	}

	mActivePawnIndex++;
	if(mActivePawnIndex >= mPlayers.size())
	{
		mActivePawnIndex = 0;
	}
	mpActivePawn = mPlayers[mActivePawnIndex];
	mMovesRemaining = mMaxMovesPerTurn;
}

void Board::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == MOUSE_CLICK_EVENT)
	{
		const MouseClickEvent &ev = static_cast<const MouseClickEvent&>(theEvent);
		/*
		if click landed on city adjacent to active pawn, move that pawn
			if click landed on same city of active pawn and no active cards, reduce that city's disease cubes
				if click landed on a card, set that card to active and resolve that in any future clicks(eg set card to active, then click on own city to perform charter flight action, or do trades if player clicks in card area of another player, etc)
		*/
		if(Game::getInstance()->getGamestate() == PLAYING)
		{
			if(ev.getButton() == LEFT)
			{
				//if no active card
				if(mpActiveCard == nullptr)
				{
				    //  if pawn's current city was clicked
					if(mpActivePawn->getCurrentCity()->contains(ev.getPosition()))
					{
						//reduce disease cubes
						if(decrementDiseaseCubes(mpActivePawn->getCurrentCity()))
						{
							gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT));
							return;
						}
					}
					else
					{
						//for each neighbor, if it was clicked, move pawn to it and break
						for(auto &v : mpActivePawn->getCurrentCity()->getNeighbors())
						{
							if(v->contains(ev.getPosition()))
							{
								mpActivePawn->moveCity(v);
								gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT)); //if we reached this point, the pawn is guaranteed to be able to move to any neighbor, so no need for return value
								return;
							}
						}

						// For each card player owns, if it was clicked, set it to active
						for(auto &v : mpActivePawn->getHand())
						{
							if(v->contains(ev.getPosition()))
							{
								// TODO: move this to its own method and highlight the card
								mpActiveCard = v;
								return;
							}
						}
					}
				}
				else
				{
					// Poll each city, if it was clicked
						// If city matches card's city
							// Move pawn to city and discard card
					for(auto &city : mCities)
					{
						if(mpActiveCard->getCity() == city && city->contains(ev.getPosition()) && !mpActivePawn->isInCity(city))
						{
							mpActivePawn->moveCity(city);
							discardPlayerCard(mpActivePawn, mpActiveCard);
							gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT));
							mpActiveCard = nullptr;
							return;
						}
					}

					// Bogus click, reset active card
					mpActiveCard = nullptr;
					return;
				}
			}
			else if(ev.getButton() == RIGHT)
			{
				//DEBUG
				//if any city was clicked, increment its cubes
				for(auto &v : mCities)
				{
					if(v->contains(ev.getPosition()))
					{
						v->incrementDiseaseCubes(1);
						break;
					}
				}
			}
		}
	}
	else if(theEvent.getType() == DECREMENT_MOVES_EVENT)
	{
		//TODO: move to new function(s) for decrement moves and end turn
		//adjust moves and change active pawn if needed
		mMovesRemaining--;
		if(mMovesRemaining <= 0)
		{
			endTurn();
		}
	}
}

const int Board::getMovesRemaining()
{
	return mMovesRemaining;
}
