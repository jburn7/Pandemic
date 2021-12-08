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
public:
	Board();
	~Board();

	void init(int numPlayers);

	//event functions
	virtual void handleEvent(const Event& theEvent);

	//getters
	const int getMovesRemaining();

private:
	void cleanup();
	void dealInitialPlayerCards();
	void dealTopPlayerCard(Player *player);
	bool decrementDiseaseCubes(City *city);
	void discardPlayerCard(Player* player, PlayerCard* card);
	void discardInfectionCard(InfectionCard *card);
	void drawInfectionCard(int numCubesToAdd);
	void doleInitialDiseaseCubes();
	void endGameAndRestart();
	void endTurn();
	void placeInfectionCardOntoDeck(InfectionCard *card);
	bool checkDeckForClick(const std::vector<PlayerCard*> &deck, Vector2D pos, const std::string &opener);
	bool checkDeckForClick(const std::vector<InfectionCard*> &deck, Vector2D pos, const std::string &opener); // Sure would be cool if C++ could cast from vec<Derived*> to vec<Base*>...
	void shuffleDrawPiles();

	PlayerCard *mpActiveCard; //user clicks this card then a city to perform an action, so we store a pointer between those clicks
	Player *mpActivePawn; //switches after end of each turn. don't worry about null checking this because if it ever equals null then we have bigger problems
	unsigned int mActivePawnIndex; //lets mpActivePawn switch to next pawn in array

	int mMovesRemaining, mMaxMovesPerTurn, mNumPlayerCardsToDraw;

	// TODO: store pannable size of board and prevent panning outside of that

	Vector2D mPlayerDiscardLocation, mPlayerDrawLocation, mInfectionDrawLocation, mInfectionDiscardLocation;

	std::vector<EnumDiseaseStages> mDiseaseStages; // stores info on whether disease at given index is spreading, cured, or eradicated. Will correspond to mDiseaseCubesRemainingByType
	std::vector<int> mDiseaseCubesRemainingByType; // stores total number of cubes remaining per each city "color" or type. typically, 4 types
	std::vector<int> mInitNumCitiesCubes; // stores ratio of disease cubes to dole out for each infection card draw in setup phase

	std::vector<City*> mCities;
	std::vector<Player*> mPlayers;

	std::vector<PlayerCard*> playerDraw;
	std::vector<PlayerCard*> playerDiscard;
	std::vector<InfectionCard*> mInfectDraw;
	std::vector<InfectionCard*> infectDiscard;
};