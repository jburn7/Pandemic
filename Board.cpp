#include "Board.h"
#include "game.h"
#include "CameraEvents.h"
#include "ColorManager.h"
#include "graphicsSystem.h"
#include "AIEvents.h"

Board::Board()
{
}

Board::~Board()
{
	cleanup();
}

void Board::init(unsigned int numPlayers)
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	// Initialize variables
	ColorManager& colorManager = *ColorManager::getInstance();
	mActiveCardColor = colorManager.color(doc["cityCard"]["highlightColor"].GetString());

	//load cities
	rapidjson::Value &c = doc["cities"];

	//build array of neighbors for each city, as well as infection card deck and city cards for player card deck
	mPlayerDrawLocation = Vector2D(doc["game"]["playerDrawLocation"].GetArray()[0].GetFloat(), doc["game"]["playerDrawLocation"].GetArray()[1].GetFloat());
	mInfectionDrawLocation = Vector2D(doc["game"]["infectionDrawLocation"].GetArray()[0].GetFloat(), doc["game"]["infectionDrawLocation"].GetArray()[1].GetFloat());
	std::vector<std::vector<int>> neighborMap;
	int numTypes = 0; // determine number of disease types based on highest city type seen in data loading
	// TODO: add more cities, then we can add epidemics
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

		// Also generate city card deck here since each card is tied to a city
		Sprite *cityCardBackground = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png"));
		PlayerCard *pc = new PlayerCard(mPlayerDrawLocation, cityCardBackground, city);
		pc->setZLayer(3);
		playerDraw.push_back(pc);
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, pc));

		// Also generate infection card deck here since each card is tied to a city
		Sprite *infectionCardBackground = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png"));
		InfectionCard *infectionCard = new InfectionCard(mInfectionDrawLocation, infectionCardBackground, city);
		infectionCard->setZLayer(3);
		infectDraw.push_back(infectionCard);
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, infectionCard));

		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, city));
	}

	// Init disease type info
	for(int i = 0; i <= numTypes; i++)
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

	// TODO: add event cards to player deck
	shuffleDrawPiles();

	//load in players. they should all start at Atlanta according to rules, which should always be first city in array
	// Also load in colors here and set each player to its matching color in colors array if available
	std::vector<Color> playerColors;
	rapidjson::Value &colors = doc["pawn"]["playerColors"];
	
	for(auto &v : colors.GetArray())
	{
		playerColors.push_back(Color(colorManager.color(v.GetString())));
	}
	// TODO: add support for more than 1 player (switch after each turn, display next player's hand, remove old player's hand from click handler, etc)
	const Vector2D playerHandLocation = Vector2D(doc["game"]["playerHandLocation"]["x"].GetFloat(), doc["game"]["playerHandLocation"]["y"].GetFloat());
	for(unsigned int i = 0; i < numPlayers; i++)
	{
		Player *p = new Player(mCities[0], std::vector<PlayerCard*>(), playerHandLocation, new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pawn"]["pawnSprite"].GetString())));
		if(playerColors.size() > i)
		{
			p->setColor(playerColors[i]);
		}
		mPlayers.push_back(p);
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, p));
	}

	mpActiveCard = nullptr;
	mActivePawnIndex = 0;
	mpActivePawn = mPlayers[mActivePawnIndex];

	dealInitialPlayerCards();

	mPlayerDiscardLocation = Vector2D(doc["game"]["playerDiscardLocation"].GetArray()[0].GetFloat(), doc["game"]["playerDiscardLocation"].GetArray()[1].GetFloat());
	mInfectionDiscardLocation = Vector2D(doc["game"]["infectionDiscardLocation"].GetArray()[0].GetFloat(), doc["game"]["infectionDiscardLocation"].GetArray()[1].GetFloat());

	int deckNameFontSize = doc["deck"]["nameFontSize"].GetInt();
	float deckNamePadding = doc["deck"]["namePadding"].GetFloat();
	int cardSize = playerDraw[0]->getHeight(); // All cards are same height so just grab height from a card that we know exists
	mPlayerDrawNameText = createDeckNameText(
		doc, 
		"Player Draw", 
		Vector2D(mPlayerDrawLocation.getX(), mPlayerDrawLocation.getY() + cardSize + deckNameFontSize), 
		deckNamePadding, 
		deckNameFontSize);

	mPlayerDiscardNameText = createDeckNameText(
		doc,
		"Player Discard",
		Vector2D(mPlayerDiscardLocation.getX(), mPlayerDiscardLocation.getY() + cardSize + deckNameFontSize),
		deckNamePadding,
		deckNameFontSize);

	mInfectionDrawNameText = createDeckNameText(
		doc,
		"Infection Draw",
		Vector2D(mInfectionDrawLocation.getX(), mInfectionDrawLocation.getY() + cardSize + deckNameFontSize),
		deckNamePadding,
		deckNameFontSize);

	mInfectionDiscardNameText = createDeckNameText(
		doc,
		"Infection Discard",
		Vector2D(mInfectionDiscardLocation.getX(), mInfectionDiscardLocation.getY() + cardSize + deckNameFontSize),
		deckNamePadding,
		deckNameFontSize);

	// Initialize disease cubes on cities
	for(const auto &i : doc["game"]["initNumCitiesCubes"].GetArray())
	{
		mInitNumCitiesCubes.push_back(i.GetUint());
	}
	doleInitialDiseaseCubes();

	mMaxMovesPerTurn = mMovesRemaining = doc["game"]["movesPerTurn"].GetInt();
	mNumPlayerCardsToDraw = doc["game"]["numPlayerCardsToDraw"].GetInt();

	gpEventSystem->addListener(DECREMENT_MOVES_EVENT, this);
	gpEventSystem->addListener(MOUSE_CLICK_EVENT, this);
	gpEventSystem->addListener(KEY_PRESSED_EVENT, this);
	gpEventSystem->addListener(AI_PLAYER_CUBE_EVENT, this);
	gpEventSystem->addListener(AI_PLAYER_MOVE_EVENT, this);
	gpEventSystem->addListener(AI_SHOULD_MOVE_EVENT, this);
}

void Board::activatePlayerCard(PlayerCard* card)
{
	mpActiveCard = card;
	// TODO: store highlight color as member rather than explicitly getting it here
	mpActiveCard->setColor(ColorManager::getInstance()->teal);
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

	for(auto &v : playerDraw)
	{
		v = nullptr;
	}
	playerDraw.clear();

	for(auto &v : playerDiscard)
	{
		v = nullptr;
	}
	playerDiscard.clear();

	for(auto &v : infectDraw)
	{
		v = nullptr;
	}
	infectDraw.clear();

	for(auto &v : infectDiscard)
	{
		v = nullptr;
	}
	infectDiscard.clear();

	if(mPlayerDrawNameText)
	{
		// delete handled by unit manager
		mPlayerDiscardNameText = nullptr;
	}
	if(mPlayerDiscardNameText)
	{
		// delete handled by unit manager
		mPlayerDiscardNameText = nullptr;
	}
	if(mInfectionDrawNameText)
	{
		// delete handled by unit manager
		mInfectionDrawNameText = nullptr;
	}
	if(mInfectionDiscardNameText)
	{
		// delete handled by unit manager
		mInfectionDiscardNameText = nullptr;
	}
}

UIBox* Board::createDeckNameText(const rapidjson::Document &doc, const std::string &name, const Vector2D location, const int deckNamePadding, const int deckNameFontSize)
{
	ColorManager& colorManager = *ColorManager::getInstance();
	UIBox *deckNameText = new UIBox(
		Vector2D(location.getX(), location.getY()),
		deckNameFontSize,
		Vector2D(0, 0),
		colorManager.color(doc["ui"]["defaultUIColor"].GetString()),
		deckNamePadding,
		name,
		new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["deck"]["nameBackgroundImg"].GetString())),
		new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["ui"]["defaultUIPaddingImage"].GetString()))
	); //deletes called in this dtor, UIBox dtor
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, deckNameText));

	return deckNameText;
}

void Board::dealInitialPlayerCards()
{
	//now deal player cards to players
	unsigned int initialHandSize;
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
}

void Board::dealTopPlayerCard(Player *player)
{
	if(playerDraw.size() > 0)
	{
		PlayerCard* pc = *playerDraw.begin();
		pc->setIsHidden(false);
		player->dealCard(pc);
		playerDraw.erase(playerDraw.begin());
		if(playerDraw.size() > 0)
		{
			playerDraw[0]->setIsHidden(false);
		}
	}
	else
	{
		endGameAndRestart();
	}
}

bool Board::decrementDiseaseCubes(City* const city)
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

void Board::decrementDiseaseCubesMove(City *const city)
{
	if(decrementDiseaseCubes(mpActivePawn->getCurrentCity()))
	{
		gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT));
		return;
	}
}

void Board::decrementRemainingMoves()
{
	// Adjust moves and end turn if needed
	mMovesRemaining--;
	if(mMovesRemaining <= 0)
	{
		endTurn();
	}
}

void Board::discardPlayerCard(Player *player, PlayerCard *card)
{
	player->discardCard(card);
	card->setPosition(mPlayerDiscardLocation);
	if(playerDiscard.size() > 0)
	{
		playerDiscard[playerDiscard.size() - 1]->setIsHidden(true);
	}
	playerDiscard.push_back(card);
}

void Board::discardInfectionCard(InfectionCard *card)
{
	card->setPosition(mInfectionDiscardLocation);
	if(infectDiscard.size() > 0)
	{
		infectDiscard[infectDiscard.size() - 1]->setIsHidden(true);
	}
	card->setIsHidden(false);
	infectDiscard.push_back(card);
}

void Board::drawInfectionCard(int numCubesToAdd)
{
	InfectionCard *card = *infectDraw.begin();
	card->getCity()->incrementDiseaseCubes(numCubesToAdd);
	infectDraw.erase(infectDraw.begin());
	if(infectDraw.size() > 0)
	{
		infectDraw[0]->setIsHidden(false);
	}
	discardInfectionCard(card);
}

void Board::doleInitialDiseaseCubes()
{
	//for i where i = initNumCitiesCubes[j], give random city j + 1 cubes
	unsigned int numInitCubes = 1;
	for(const auto &i : mInitNumCitiesCubes)
	{
		int numCities = i;
		for(int j = 0; j < numCities; j++)
		{
			drawInfectionCard(numInitCubes);
		}
		numInitCubes++;
	}
}

void Board::endGameAndRestart()
{
	//TODO: end game as loss
	std::cout << "TODO: you lost\n";

	// move all discarded cards to their draw piles
	for(auto &v : playerDiscard)
	{
		v->setIsHidden(true);
		v->setPosition(mPlayerDrawLocation);
		if(playerDraw.size() > 0)
		{
			playerDraw[0]->setIsHidden(true);
		}
		playerDraw.push_back(v);
	}
	playerDiscard.clear();
	for(auto &v : infectDiscard)
	{
		placeInfectionCardOntoDeck(v);
	}
	infectDiscard.clear();
	// take all player cards from players' hands and replace them in player draw
	for(auto &v : mPlayers)
	{
		v->replaceHandIntoDeck(playerDraw, mPlayerDrawLocation);
	}
	// reset each city's cubes to 0
	for(auto &v : mCities)
	{
		v->clearAllCubes();
	}
	// reshuffle draw piles
	shuffleDrawPiles();
	// redraw player cards
	dealInitialPlayerCards();
	// redraw infection cards
	doleInitialDiseaseCubes();
	// move players to Atlanta
	for(auto &v : mPlayers)
	{
		v->moveCity(mCities[0]);
	}
	// reset moveCount
	mMovesRemaining = mMaxMovesPerTurn;
	// reset activePawn to starting player
	mpActivePawn = mPlayers[0];
	// TODO: other functionality (eg resetting counters, epidemic states, etc)
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

	for(int i = 0; i < mNumPlayerCardsToDraw; i++)
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

// Wraps flying, nulling active card, and discarding
void Board::flyToCity(City* const city)
{
	mpActivePawn->moveCity(city);
	discardPlayerCard(mpActivePawn, mpActiveCard);
	gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT));
	mpActiveCard->setColor(ColorManager::getInstance()->black);
	mpActiveCard = nullptr;
}

void Board::placeInfectionCardOntoDeck(InfectionCard *card)
{
	if(infectDraw.size() > 0)
	{
		infectDraw[0]->setIsHidden(true);
	}
	card->setPosition(mInfectionDrawLocation);
	infectDraw.insert(infectDraw.begin(), card);
}

bool Board::checkDeckForClick(const std::vector<PlayerCard*> &deck, Vector2D pos, const std::string &opener)
{
	// Hacky, but we'll just ask the top card in each stack for its position
	if(deck.size() > 0)
	{
		if(deck[0]->contains(pos))
		{
			std::cout << opener << std::endl;

			for(auto &a : deck)
			{
				std::cout << a->debugDescription() << std::endl;
			}

			return true;
		}
	}

	return false;
}

bool Board::checkDeckForClick(const std::vector<InfectionCard*> &deck, Vector2D pos, const std::string &opener)
{
	// Hacky, but we'll just ask the top card in each stack for its position
	if(deck.size() > 0)
	{
		if(deck[0]->contains(pos))
		{
			std::cout << opener << std::endl;

			for(auto &a : deck)
			{
				std::cout << a->debugDescription() << std::endl;
			}

			return true;
		}
	}

	return false;
}

void Board::shuffleDrawPiles()
{
	std::random_shuffle(playerDraw.begin(), playerDraw.end());
	std::random_shuffle(infectDraw.begin(), infectDraw.end());
}

void Board::handleEvent(const Event &theEvent)
{
	const Gamestate gameState = Game::getInstance()->getGamestate();
	if(theEvent.getType() == MOUSE_CLICK_EVENT)
	{
		const MouseClickEvent &ev = static_cast<const MouseClickEvent&>(theEvent);
		// Two different coordinates for the two different views, will need to track both and then each unit here can decide which one to use based on whether it is a gui unit
		Vector2D basePos = Game::getInstance()->getGraphics().convertToWorldCoordinates(ev.getPosition(), BASE_VIEW);
		Vector2D guiPos = Game::getInstance()->getGraphics().convertToWorldCoordinates(ev.getPosition(), GUI_VIEW);
		/*
		if click landed on city adjacent to active pawn, move that pawn
			if click landed on same city of active pawn and no active cards, reduce that city's disease cubes
				if click landed on a card, set that card to active and resolve that in any future clicks(eg set card to active, then click on own city to perform charter flight action, or do trades if player clicks in card area of another player, etc)
		*/
		if(gameState == PLAYING)
		{
			if(ev.getButton() == MOUSE_LEFT)
			{
				// DEBUG: if click lands on a draw/discard pile, then just print its contents for now
				// TODO: find a way to show this graphically
				std::cout << "EVENT: Left click at " << "(" << ev.getPosition().getX() << ", " << ev.getPosition().getY() << ")" << std::endl;
				std::cout << "\t World coords at " << "(" << basePos.getX() << ", " << basePos.getY() << ")" << std::endl;
				std::cout << "\t GUI coords at " << "(" << guiPos.getX() << ", " << guiPos.getY() << ")" << std::endl;
				if(checkDeckForClick(playerDraw, guiPos, "Player Draw contents:"))
				{
					return;
				}
				if(checkDeckForClick(playerDiscard, guiPos, "Player Discard contents:"))
				{
					return;
				}
				if(checkDeckForClick(infectDraw, guiPos, "Infection Draw contents:"))
				{
					return;
				}
				if(checkDeckForClick(infectDiscard, guiPos, "Infection Discard contents:"))
				{
					return;
				}

				//if no active card
				if(mpActiveCard == nullptr)
				{
				    //  if pawn's current city was clicked
					if(mpActivePawn->getCurrentCity()->contains(basePos))
					{
						decrementDiseaseCubesMove(mpActivePawn->getCurrentCity());
					}
					else
					{
						//for each neighbor, if it was clicked, move pawn to it and break
						for(auto &v : mpActivePawn->getCurrentCity()->getNeighbors())
						{
							if(v->contains(basePos))
							{
								mpActivePawn->moveCity(v);
								gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT)); //if we reached this point, the pawn is guaranteed to be able to move to any neighbor, so no need for return value
								return;
							}
						}

						// For each card player owns, if it was clicked, set it to active
						for(auto &v : mpActivePawn->getHand())
						{
							if(v->contains(guiPos))
							{
								activatePlayerCard(v);
								return;
							}
						}
					}
				}
				else
				{
					bool isCharterFlight = mpActivePawn->isInCity(mpActiveCard->getCity());
					// Poll each city, if it was clicked
					    // If charter flight, then move pawn to city and discard card
						// Else if city matches card's city
							// Move pawn to city and discard card
					for(auto &city : mCities)
					{
						if(city->contains(basePos))
						{
							if((isCharterFlight || mpActiveCard->getCity() == city) && !mpActivePawn->isInCity(city))
							{
								flyToCity(city);
								return;
							}
						}
					}

					// Bogus click, reset active card
					mpActiveCard->setColor(ColorManager::getInstance()->black);
					mpActiveCard = nullptr;
					return;
				}
			}
			else if(ev.getButton() == MOUSE_RIGHT)
			{
				//DEBUG
				//if any city was clicked, increment its cubes
				for(auto &v : mCities)
				{
					if(v->contains(basePos))
					{
						v->incrementDiseaseCubes(1);
						return;
					}
				}
			}
		}
	}
	else if(theEvent.getType() == DECREMENT_MOVES_EVENT)
	{
		decrementRemainingMoves();
	}
	else if(theEvent.getType() == AI_PLAYER_CUBE_EVENT)
	{
		if(gameState == PLAYING) {
			const AIPlayerCubeEvent &ev = static_cast<const AIPlayerCubeEvent&>(theEvent);
			if(ev.getCity()->getNumberOfDiseaseCubes() > 0)
			{
				decrementDiseaseCubesMove(ev.getCity());
			}
		}
	}
	else if(theEvent.getType() == AI_PLAYER_MOVE_EVENT)
	{
		if(gameState == PLAYING)
		{
			const AIPlayerMoveEvent &ev = static_cast<const AIPlayerMoveEvent&>(theEvent);
			bool isNeighbor = false;
			// TODO: write helper funcs for vec like .contains
			City *const currentCity = mpActivePawn->getCurrentCity();
			for(auto &c : currentCity->getNeighbors())
			{
				if(c == ev.getCity())
				{
					isNeighbor = true;
				}
			}
			if(isNeighbor)
			{
				mpActivePawn->moveCity(ev.getCity());
				gpEventSystem->fireEvent(new Event(DECREMENT_MOVES_EVENT));
			}
		}
	}
}

const int Board::getMovesRemaining()
{
	return mMovesRemaining;
}
