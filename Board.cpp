#include "Board.h"
#include "game.h"
#include "CameraEvents.h"
#include "ColorManager.h"
#include "graphicsSystem.h"
#include "KeyEvent.h"
#include "AIEvents.h"
#include "Deck.h"
#include "PlayerCardDeck.h"
#include "InfectionCardDeck.h"
#include "PawnChangeEvents.h"
#include "PlayerInfo.h"

Board::Board()
{
	mActivePawnIndex = 0;
	mBoardOutline = nullptr;
	mInfectionDiscardDeck = nullptr;
	mInfectionDrawDeck = nullptr;
	mMaxMovesPerTurn = 0;
	mMovesRemaining = 0;
	mNumEpidemicsHad = 0;
	mNumPlayerCardsToDraw = 0;
	mOutbreakThreshold = 0;
	mPlayerDiscardDeck = nullptr;
	mPlayerDrawDeck = nullptr;
	mSelectedPawnIndex = 0;
	mpActiveCard = nullptr;
	mpActivePawn = nullptr;
	mpSelectedPawn = nullptr;
}

Board::~Board()
{
	cleanup();
}

void Board::init()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	// Initialize variables
	ColorManager& colorManager = *ColorManager::getInstance();
	mActiveCardColor = colorManager.color(doc["cityCard"]["highlightColor"].GetString());

	rapidjson::Value &epidemicInfectionCounts = doc["game"]["infectionsAfterEpidemic"];
	for(auto &v : epidemicInfectionCounts.GetArray())
	{
		mInfectionsAfterEpidemic.push_back(v.GetInt());
	}

	//load cities
	rapidjson::Value &c = doc["cities"];

	mStartingCity = doc["game"]["startingCity"].GetString();
	mOutbreakThreshold = doc["city"]["outbreakThreshold"].GetInt();

	mPlayerDrawDeck = new PlayerCardDeck(
		doc,
		Vector2D(doc["game"]["playerDrawLocation"].GetArray()[0].GetFloat(), doc["game"]["playerDrawLocation"].GetArray()[1].GetFloat()),
		"Player Draw"
	);
	mPlayerDiscardDeck = new PlayerCardDeck(
		doc,
		Vector2D(doc["game"]["playerDiscardLocation"].GetArray()[0].GetFloat(), doc["game"]["playerDiscardLocation"].GetArray()[1].GetFloat()),
		"Player Discard"
	);
	mInfectionDrawDeck = new InfectionCardDeck(
		doc,
		Vector2D(doc["game"]["infectionDrawLocation"].GetArray()[0].GetFloat(), doc["game"]["infectionDrawLocation"].GetArray()[1].GetFloat()),
		"Infection Draw"
	);
	mInfectionDiscardDeck = new InfectionCardDeck(
		doc,
		Vector2D(doc["game"]["infectionDiscardLocation"].GetArray()[0].GetFloat(), doc["game"]["infectionDiscardLocation"].GetArray()[1].GetFloat()),
		"Infection Discard"
	);

	//build array of neighbors for each city, as well as infection card deck and city cards for player card deckS
	std::vector<std::vector<std::string>> neighborMap;
	int numTypes = 0; // determine number of disease types based on highest city type seen in data loading
	const Vector2D playerDrawLocation = mPlayerDrawDeck->getPosition();
	const Vector2D infectionDrawLocation = mInfectionDrawDeck->getPosition();
	// TODO: add rest of cities
	for(auto &v : c.GetArray())
	{
		int type = v["type"].GetInt();
		if(type > numTypes)
		{
			numTypes = type;
		}
		std::string cityName = v["name"].GetString();
		City *city = new City(cityName, type, Vector2D(v["posX"].GetFloat(), v["posY"].GetFloat()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city.png"))); //deletes will be called in UnitManager destructor and Unit destructor respectively
		city->setScale(1, 1);
		mCities.insert(CityItem(cityName, city));
		neighborMap.push_back(std::vector<std::string>());
		for(auto &n : v["neighbors"].GetArray())
		{
			neighborMap.back().push_back(n.GetString());
		}

		// Also generate city card deck here since each card is tied to a city
		Sprite *cityCardBackground = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png"));
		PlayerCard *pc = new PlayerCard(playerDrawLocation, cityCardBackground, city);
		pc->setZLayer(3);
		mPlayerDrawDeck->addCard(pc);
		gpEventSystem->fireEvent(new UnitAddEvent(pc));

		// Also generate infection card deck here since each card is tied to a city
		Sprite *infectionCardBackground = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png"));
		InfectionCard *infectionCard = new InfectionCard(infectionDrawLocation, infectionCardBackground, city);
		infectionCard->setZLayer(3);
		mInfectionDrawDeck->addCard(infectionCard);
		gpEventSystem->fireEvent(new UnitAddEvent(infectionCard));

		gpEventSystem->fireEvent(new UnitAddEvent(city));
	}

	// Init disease type info
	for(int i = 0; i <= numTypes; i++)
	{
		mDiseaseStages.insert(std::pair(CityType(i), DiseaseStages::Spreading));
		mDiseaseCubesRemainingByType.insert(std::pair(CityType(i), 0));
	}

	//now that all cities neighbors are loaded, find their neighbors and set the pointers
	//neighbor corresponds to position in JSON array, and thus position in mCities
	int cityIndex = 0;
	for(auto &c : mCities)
	{
		City* city = c.second;
		city->loadNeighbors(mCities, neighborMap[cityIndex]);
		cityIndex++;
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
	// TODO: wrap cards if player's hand goes off screen (lower priority, screen has plenty of room)
	const Vector2D playerHandLocation = Vector2D(doc["game"]["playerHandLocation"]["x"].GetFloat(), doc["game"]["playerHandLocation"]["y"].GetFloat());
	const Vector2D playerInfoLocation = Vector2D(doc["game"]["playerInfoLocation"]["x"].GetFloat(), doc["game"]["playerInfoLocation"]["y"].GetFloat());
	const float playerInfoSpriteScale = doc["game"]["playerInfoSpriteScale"].GetFloat();
	unsigned int numPlayers = doc["game"]["numPlayers"].GetUint();
	for(unsigned int i = 0; i < numPlayers; i++)
	{
		Sprite* pawnSprite = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pawn"]["pawnSprite"].GetString()));
		Player *p = new Player(mCities.at(mStartingCity), std::vector<PlayerCard*>(), playerHandLocation, pawnSprite);
		if(playerColors.size() > i)
		{
			p->setColor(playerColors[i]);
		}
		mPlayers.push_back(p);
		gpEventSystem->fireEvent(new UnitAddEvent(p));

		PlayerInfo *playerInfo = new PlayerInfo(
			p,
			playerInfoLocation + Vector2D((playerInfoSpriteScale * pawnSprite->getHeight() + 80) * i, 0),
			playerInfoSpriteScale,
			Outline(Color(200, 100, 69), Color(0, 0, 0), 5)); // TODO: remove hardcoded values
		playerInfo->setIsGuiLayer(true);
		mPlayerInfos.push_back(playerInfo);
		gpEventSystem->fireEvent(new UnitAddEvent(playerInfo));
	}
	mPlayerInfos[0]->setSelected(true);

	mpActiveCard = nullptr;
	changeActivePawn(0);
	// Can't use changeSelecedPawn until player's hand is initially set
	mSelectedPawnIndex = 0;
	mpSelectedPawn = mpActivePawn;

	dealInitialPlayerCards();

	// Initialize disease cubes on cities
	for(const auto &i : doc["game"]["initNumCitiesCubes"].GetArray())
	{
		mInitNumCitiesCubes.push_back(i.GetUint());
	}
	doleInitialDiseaseCubes();

	mMaxMovesPerTurn = mMovesRemaining = doc["game"]["movesPerTurn"].GetInt();
	mNumPlayerCardsToDraw = doc["game"]["numPlayerCardsToDraw"].GetInt();

	// Board window outline
	GraphicsSystem *graphics = &Game::getInstance()->getGraphics();
	float boardStartX = doc["window"]["boardViewport"]["startX"].GetFloat() * graphics->getWidth();
	float boardStartY = doc["window"]["boardViewport"]["startY"].GetFloat() * graphics->getHeight();
	float boardWidth = doc["window"]["boardViewport"]["widthRatio"].GetFloat() * graphics->getWidth();
	float boardHeight = doc["window"]["boardViewport"]["heightRatio"].GetFloat() * graphics->getHeight();
	mBoardOutline = new Unit(Vector2D(boardStartX, boardStartY), (int)boardWidth, (int)boardHeight);
	mBoardOutline->setOutline(Outline(colorManager.color(doc["window"]["boardOutline"]["color"].GetString()), colorManager.clear, doc["window"]["boardOutline"]["thickness"].GetInt()));
	mBoardOutline->setIsGuiLayer(true);
	gpEventSystem->fireEvent(new UnitAddEvent(mBoardOutline));

	mPlayerDrawDeck->completeInit();

	gpEventSystem->addListener(EventType::DECREMENT_MOVES_EVENT, this);
	gpEventSystem->addListener(EventType::MOUSE_CLICK_EVENT, this);
	gpEventSystem->addListener(EventType::KEY_PRESSED_EVENT, this);
	gpEventSystem->addListener(EventType::AI_PLAYER_CUBE_EVENT, this);
	gpEventSystem->addListener(EventType::AI_PLAYER_MOVE_EVENT, this);
	gpEventSystem->addListener(EventType::EPIDEMIC_EVENT, this);
}

void Board::activatePlayerCard(PlayerCard* card)
{
	mpActiveCard = card;
	mpActiveCard->setColor(mActiveCardColor);
}

void Board::cleanup()
{
	for(auto &v : mCities)
	{
		v.second = nullptr;
	}
	mCities.clear();

	for(auto &v : mPlayers)
	{
		v = nullptr;
	}
	mPlayers.clear();

	if(mPlayerDrawDeck)
	{
		delete mPlayerDrawDeck;
		mPlayerDrawDeck = NULL;
	}
	if(mPlayerDiscardDeck)
	{
		delete mPlayerDiscardDeck;
		mPlayerDiscardDeck = NULL;
	}
	if(mInfectionDrawDeck)
	{
		delete mInfectionDrawDeck;
		mInfectionDrawDeck = NULL;
	}
	if(mInfectionDiscardDeck)
	{
		delete mInfectionDiscardDeck;
		mInfectionDiscardDeck = NULL;
	}
}

void Board::changeActivePawn(int newIndex)
{
	mActivePawnIndex = newIndex;
	mpActivePawn = mPlayers[mActivePawnIndex];
	gpEventSystem->fireEvent(new ActivePawnChangeEvent(*mpActivePawn));
}

void Board::changeSelectedPawn(int newIndex)
{
	for(auto &v : mpSelectedPawn->getHand())
	{
		v->setIsHidden(true);
	}
	mSelectedPawnIndex = newIndex;
	mpSelectedPawn = mPlayers[mSelectedPawnIndex];
	for(auto &v : mpSelectedPawn->getHand())
	{
		v->setIsHidden(false);
	}
	for(auto& v : mPlayerInfos)
	{
		v->setSelected(false);
	}
	mPlayerInfos[mSelectedPawnIndex]->setSelected(true);
	gpEventSystem->fireEvent(new SelectedPawnChangeEvent(*mpActivePawn));
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
		initialHandSize = 1;
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
		mpSelectedPawn = mPlayers[i];
		for(unsigned int j = 0; j < initialHandSize; j++)
		{
			dealTopPlayerCard(mpSelectedPawn, i == 0);
		}
	}
	mpSelectedPawn = mpActivePawn;
}

void Board::dealTopPlayerCard(Player *player, bool showCard)
{
	PlayerCard* pc = mPlayerDrawDeck->dealTopCard();
	if(pc)
	{
		pc->setIsHidden(!showCard);
		if(pc->getCardType() == PlayerCardType::EPIDEMIC)
		{
			// TODO: we might want to do stuff with the card later, but for now just fire an event and forget
			gpEventSystem->fireEvent(new Event(EventType::EPIDEMIC_EVENT));
			delete pc;
		}
		else
		{
			player->dealCard(pc);
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
		// TODO: this logic is wrong, needs to check all cities of that disease type
		if(mDiseaseStages[city->getType()] == DiseaseStages::Cured)
		{
			mDiseaseStages[city->getType()] = DiseaseStages::Eradicated;
		}
	}
	return didDecrement;
}

void Board::decrementDiseaseCubesMove(City *const city)
{
	if(decrementDiseaseCubes(mpActivePawn->getCurrentCity()))
	{
		gpEventSystem->fireEvent(new Event(EventType::DECREMENT_MOVES_EVENT));
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
	mPlayerDiscardDeck->addCard(card);
}

void Board::discardInfectionCard(InfectionCard *card)
{
	mInfectionDiscardDeck->addCard(card);
}

void Board::drawInfectionCard(int numCubesToAdd)
{
	InfectionCard* card = mInfectionDrawDeck->dealTopCard();
	if(card)
	{
		card->getCity()->incrementDiseaseCubes(numCubesToAdd);
		discardInfectionCard(card);
	}
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
	PlayerCard* pc = mPlayerDiscardDeck->dealTopCard();
	while(pc)
	{
		mPlayerDrawDeck->addCard(pc);
		pc = mPlayerDiscardDeck->dealTopCard();
	}
	InfectionCard* card = mInfectionDiscardDeck->dealTopCard();
	while(pc)
	{
		mInfectionDrawDeck->addCard(card);
		card = mInfectionDiscardDeck->dealTopCard();
	}
	// take all player cards from players' hands and replace them in player draw
	for(auto &v : mPlayers)
	{
		v->replaceHandIntoDeck(mPlayerDrawDeck);
	}
	// reset each city's cubes to 0
	for(auto &v : mCities)
	{
		v.second->clearAllCubes();
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
		v->moveCity(mCities.at(mStartingCity));
	}
	// reset moveCount
	mMovesRemaining = mMaxMovesPerTurn;
	// reset activePawn to starting player
	mpActivePawn = mPlayers[0];
	// TODO: other functionality (eg resetting counters, epidemic states, etc)
	mNumEpidemicsHad = 0;
	mPlayerDrawDeck->completeInit();
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
	if((unsigned int)mActivePawnIndex >= mPlayers.size())
	{
		mActivePawnIndex = 0;
	}
	changeActivePawn(mActivePawnIndex);
	changeSelectedPawn(mActivePawnIndex);
	mMovesRemaining = mMaxMovesPerTurn;
}

// Wraps flying, nulling active card, and discarding
void Board::flyToCity(City* const city)
{
	mpActivePawn->moveCity(city);
	discardPlayerCard(mpActivePawn, mpActiveCard);
	gpEventSystem->fireEvent(new Event(EventType::DECREMENT_MOVES_EVENT));
	mpActiveCard->setColor(ColorManager::getInstance()->white);
	mpActiveCard = nullptr;
}

void Board::placeInfectionCardOntoDeck(InfectionCard *card)
{
	mInfectionDiscardDeck->addCard(card);
}

void Board::resetActiveCard()
{
	mpActiveCard->setColor(ColorManager::getInstance()->white);
	mpActiveCard = nullptr;
}

void Board::shuffleDrawPiles()
{
	mPlayerDrawDeck->shuffle();
	mInfectionDrawDeck->shuffle();
}

void Board::handleGuiClick(Vector2D guiPos)
{
	// TODO: card trading (drag and drop onto PlayerInfo, context menu on card click, what other options are there?
	// Hand click, deck click
	if(mPlayerDrawDeck->checkDeckForClick(guiPos, "Player Draw contents:"))
	{
		return;
	}
	if(mPlayerDiscardDeck->checkDeckForClick(guiPos, "Player Discard contents:"))
	{
		return;
	}
	if(mInfectionDrawDeck->checkDeckForClick(guiPos, "Infection Draw contents:"))
	{
		return;
	}
	if(mInfectionDiscardDeck->checkDeckForClick(guiPos, "Infection Discard contents:"))
	{
		return;
	}

	// TODO: allow selecting of other hands's cards for things like trading, but check whether player actually owns card before playing it

	// Selected hand click
	for(unsigned int i = 0; i < mPlayerInfos.size(); i++)
	{
		if(mPlayerInfos[i]->contains(guiPos))
		{
			// If player has an active card selected and it can be traded to the selected player, then trade it
			// TODO: modify to allow for taking cards as well as giving away
			Player *targetPlayer = mPlayerInfos[i]->getPlayer();
			if(mpActiveCard &&
			   (mpActivePawn != targetPlayer && mpActivePawn->hasCard(mpActiveCard) && mpActivePawn->tradeCard(mpActiveCard, targetPlayer) ||
			   (mpActivePawn == targetPlayer && mpActivePawn != mpSelectedPawn && mpSelectedPawn->hasCard(mpActiveCard) && mpSelectedPawn->tradeCard(mpActiveCard, targetPlayer))))
			{
				resetActiveCard();
				gpEventSystem->fireEvent(new Event(EventType::DECREMENT_MOVES_EVENT));
			}
			changeSelectedPawn(i);
			return;
		}
	}

	// If no active card
	if(mpActiveCard == nullptr)
	{
		// For each card that selected player owns, if it was clicked, set it to active
		for(auto &v : mpSelectedPawn->getHand())
		{
			if(v->contains(guiPos))
			{
				activatePlayerCard(v);
				return;
			}
		}
	}
	else
	{
		// Bogus click, reset active card
		resetActiveCard();
		return;
	}
}

void Board::handleBoardClick(Vector2D basePos)
{
	// TODO: move window when pawn moves?
	// City click
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
					gpEventSystem->fireEvent(new Event(EventType::DECREMENT_MOVES_EVENT)); //if we reached this point, the pawn is guaranteed to be able to move to any neighbor, so no need for return value
					return;
				}
			}
		}
	}
	else if(mpActivePawn->hasCard(mpActiveCard))
	{
		bool isCharterFlight = mpActivePawn->isInCity(mpActiveCard->getCity());
		// Poll each city, if it was clicked
		// If charter flight, then move pawn to city and discard card
		// Else if city matches card's city
		// Move pawn to city and discard card
		for(auto &c : mCities)
		{
			City* const city = c.second;
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
		resetActiveCard();
		return;
	}
}

void Board::incrementSelectedPawn(int increment)
{
	int newIndex = mSelectedPawnIndex + increment;
	if(newIndex < 0)
	{
		newIndex = mPlayers.size() - 1;
	}
	else if((unsigned int)newIndex >= mPlayers.size())
	{
		newIndex = 0;
	}
	changeSelectedPawn(newIndex);
}

void Board::handleEvent(const Event &theEvent)
{
	const Gamestate gameState = Game::getInstance()->getGamestate();
	if(theEvent.getType() == EventType::MOUSE_CLICK_EVENT)
	{
		const MouseClickEvent &ev = static_cast<const MouseClickEvent&>(theEvent);
		// Two different coordinates for the two different views, will need to track both and then each unit here can decide which one to use based on whether it is a gui unit
		Vector2D basePos = Game::getInstance()->getGraphics().convertToWorldCoordinates(ev.getPosition(), GraphicsLayer::BASE_VIEW);
		Vector2D guiPos = Game::getInstance()->getGraphics().convertToWorldCoordinates(ev.getPosition(), GraphicsLayer::GUI_VIEW);
		/*
		if click landed on city adjacent to active pawn, move that pawn
			if click landed on same city of active pawn and no active cards, reduce that city's disease cubes
				if click landed on a card, set that card to active and resolve that in any future clicks(eg set card to active, then click on own city to perform charter flight action, or do trades if player clicks in card area of another player, etc)
		*/

		if(gameState == Gamestate::PLAYING)
		{
			if(ev.getButton() == MouseButton::LEFT)
			{
				// DEBUG: if click lands on a draw/discard pile, then just print its contents for now
				// TODO: find a way to show this graphically
				std::cout << "EVENT: Left click at " << "(" << ev.getPosition().getX() << ", " << ev.getPosition().getY() << ")" << std::endl;
				std::cout << "\t World coords at " << "(" << basePos.getX() << ", " << basePos.getY() << ")" << std::endl;
				std::cout << "\t GUI coords at " << "(" << guiPos.getX() << ", " << guiPos.getY() << ")" << std::endl;
				if(mBoardOutline->contains(guiPos))
				{
					handleBoardClick(basePos);
				}
				else
				{
					handleGuiClick(guiPos);
				}
			}
			else if(ev.getButton() == MouseButton::RIGHT)
			{
				//DEBUG
				//if any city was clicked, increment its cubes
				for(auto &c : mCities)
				{
					City* const city = c.second;
					if(city->contains(basePos))
					{
						city->incrementDiseaseCubes(1);
						return;
					}
				}
			}
		}
	}
	else if(theEvent.getType() == EventType::DECREMENT_MOVES_EVENT)
	{
		decrementRemainingMoves();
	}
	else if(theEvent.getType() == EventType::AI_PLAYER_CUBE_EVENT)
	{
		if(gameState == Gamestate::PLAYING) {
			const AIPlayerCubeEvent &ev = static_cast<const AIPlayerCubeEvent&>(theEvent);
			if(ev.getCity()->getNumberOfDiseaseCubes() > 0)
			{
				decrementDiseaseCubesMove(ev.getCity());
			}
		}
	}
	else if(theEvent.getType() == EventType::AI_PLAYER_MOVE_EVENT)
	{
		if(gameState == Gamestate::PLAYING)
		{
			const AIPlayerMoveEvent &ev = static_cast<const AIPlayerMoveEvent&>(theEvent);
			bool isNeighbor = false;
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
				gpEventSystem->fireEvent(new Event(EventType::DECREMENT_MOVES_EVENT));
			}
		}
	}
	else if(theEvent.getType() == EventType::EPIDEMIC_EVENT)
	{
		if(gameState == Gamestate::PLAYING)
		{
			mNumEpidemicsHad++;
			/*
				The basic idea here is to draw a city from the bottom of the infection deck, place three cubes on it, then reshuffle it with all discarded infection cards, place on top of infect draw, then draw X infection cards and apply all cube/outbreak effects
			*/
			InfectionCard* outbreakCity = mInfectionDrawDeck->drawLastCard();
			if(outbreakCity)
			{
				// Add three cubes to outbreak city
				outbreakCity->getCity()->setDiseaseCubes(mOutbreakThreshold);
				// add to infection discard
				mInfectionDiscardDeck->addCard(outbreakCity);
				// shuffle infection discard
				mInfectionDiscardDeck->shuffle();
				// add discarded cards to draw
				mInfectionDiscardDeck->moveContentsToDeck(mInfectionDrawDeck);
				// draw one at a time until X, resolving events for each (X varies based on game rules and epidemic count)
				for(int i = 0; i < mInfectionsAfterEpidemic[mNumEpidemicsHad]; i++)
				{
					drawInfectionCard(1); // TODO: test this event logic. Do we need to dispatch all events inside the loop, or can we just let the outreak events stack and handle them at the end of this game tick?
				}
			}
			else
			{
				// This should be impossible
				std::cerr << "Warning: infection draw deck is empty but received epidemic" << std::endl;
			}
		}
	}
}

const int Board::getNumEpidemicsHad()
{
	return mNumEpidemicsHad;
}

const int Board::getMovesRemaining()
{
	return mMovesRemaining;
}
