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
	void dealTopPlayerCard(Player *player);
	bool decrementDiseaseCubes(City *city);
	void discardPlayerCard(Player* player, PlayerCard* card);
	void endTurn();

	PlayerCard *mpActiveCard; //user clicks this card then a city to perform an action, so we store a pointer between those clicks
	Player *mpActivePawn; //switches after end of each turn. don't worry about null checking this because if it ever equals null then we have bigger problems
	unsigned int mActivePawnIndex; //lets mpActivePawn switch to next pawn in array

	int mMovesRemaining, mMaxMovesPerTurn, mNumPlayerCardsToDraw;

	Vector2D mPlayerDiscardLocation, mPlayerDrawLocation;

	std::vector<EnumDiseaseStages> mDiseaseStages; // stores info on whether disease at given index is spreading, cured, or eradicated. Will correspond to mDiseaseCubesRemainingByType
	std::vector<int> mDiseaseCubesRemainingByType; // stores total number of cubes remaining per each city "color" or type. typically, 4 types

	std::vector<City*> mCities;
	std::vector<Player*> mPlayers;

	std::vector<PlayerCard*> playerDraw;
	std::vector<PlayerCard*> playerDiscard;
	std::vector<InfectionCard*> infectDraw;
	std::vector<InfectionCard*> infectDiscard;
};