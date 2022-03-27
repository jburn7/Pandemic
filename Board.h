#pragma once
#include "City.h"
#include "InfectionCard.h"
#include "Player.h"
#include "jsonData.h"
#include "EventListener.h"
#include "event.h"
#include "DiseaseStages.h"

class Board : public EventListener
{
	friend class AISystem;
public:
	Board();
	~Board();

	void init(unsigned int numPlayers);

	//event functions
	virtual void handleEvent(const Event& theEvent);

	//getters
	const int getMovesRemaining();

private:
	void activatePlayerCard(PlayerCard* card);
	void cleanup();
	void dealInitialPlayerCards();
	void dealTopPlayerCard(Player *player);
	bool decrementDiseaseCubes(City *const city);
	void decrementDiseaseCubesMove(City *const city); // This guy will wrap the decrement moves event with the decrement cube event. Separate from the former func because we want to keep the functionality to decrement cubes without moving (i.e. event cards later)
	void decrementRemainingMoves();
	void discardPlayerCard(Player* player, PlayerCard* card);
	void discardInfectionCard(InfectionCard *card);
	void drawInfectionCard(int numCubesToAdd);
	void doleInitialDiseaseCubes();
	void endGameAndRestart();
	void endTurn();
	void flyToCity(City* const city);
	void placeInfectionCardOntoDeck(InfectionCard *card);
	bool checkDeckForClick(const std::vector<PlayerCard*> &deck, Vector2D pos, const std::string &opener);
	bool checkDeckForClick(const std::vector<InfectionCard*> &deck, Vector2D pos, const std::string &opener); // Sure would be cool if C++ could cast from vec<Derived*> to vec<Base*>...
	void shuffleDrawPiles();

	PlayerCard *mpActiveCard; //user clicks this card then a city to perform an action, so we store a pointer between those clicks
	Player *mpActivePawn; //switches after end of each turn. don't worry about null checking this because if it ever equals null then we have bigger problems
	unsigned int mActivePawnIndex; //lets mpActivePawn switch to next pawn in array

	Color mActiveCardColor;

	int mMovesRemaining, mMaxMovesPerTurn, mNumPlayerCardsToDraw;

	Vector2D mPlayerDiscardLocation, mPlayerDrawLocation, mInfectionDrawLocation, mInfectionDiscardLocation;

	std::vector<EnumDiseaseStages> mDiseaseStages; // stores info on whether disease at given index is spreading, cured, or eradicated. Will correspond to mDiseaseCubesRemainingByType
	std::vector<int> mDiseaseCubesRemainingByType; // stores total number of cubes remaining per each city "color" or type. typically, 4 types
	std::vector<int> mInitNumCitiesCubes; // stores ratio of disease cubes to dole out for each infection card draw in setup phase

	std::vector<City*> mCities;
	std::vector<Player*> mPlayers;

	std::vector<PlayerCard*> playerDraw;
	std::vector<PlayerCard*> playerDiscard;
	std::vector<InfectionCard*> infectDraw;
	std::vector<InfectionCard*> infectDiscard;
};