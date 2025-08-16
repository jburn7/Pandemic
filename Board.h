#pragma once
#include "City.h"
#include "InfectionCard.h"
#include "Player.h"
#include "jsonData.h"
#include "EventListener.h"
#include "event.h"
#include "DiseaseStages.h"

typedef std::pair<std::string, City*> CityItem;

class PlayerCardDeck;
class InfectionCardDeck;

class Board : public EventListener
{
	friend class AISystem;
public:
	Board();
	~Board();

	void init();

	//event functions
	virtual void handleEvent(const Event& theEvent);

	//getters
	const int getNumEpidemicsHad();
	const int getMovesRemaining();

private:
	void activatePlayerCard(PlayerCard* card);
	void cleanup();
	void changeActivePawn(int newIndex);
	void changeSelectedPawn(int newIndex);
	void dealInitialPlayerCards();
	void dealTopPlayerCard(Player *player, bool showCard = true);
	bool decrementDiseaseCubes(City *const city);
	void decrementDiseaseCubesMove(City *const city); // This guy will wrap the decrement moves event with the decrement cube event. Separate from the former func because we want to keep the functionality to decrement cubes without using a move (i.e. event cards later)
	void decrementRemainingMoves();
	void discardPlayerCard(Player* player, PlayerCard* card);
	void discardInfectionCard(InfectionCard *card);
	void drawInfectionCard(int numCubesToAdd);
	void doleInitialDiseaseCubes();
	void endGameAndRestart();
	void endTurn();
	void flyToCity(City* const city);
	void handleGuiClick(Vector2D guiPos);
	void handleBoardClick(Vector2D basePos);
	void handleMouseMove(Vector2D basePos, Vector2D guiPos);
	void incrementSelectedPawn(int increment = 1);
	void placeInfectionCardOntoDeck(InfectionCard *card);
	void resetActiveCard();
	void setMouseHighlightedUnit(Unit* unit);
	void shuffleDrawPiles();

	PlayerCard *mpActiveCard; //user clicks this card then a city to perform an action, so we store a pointer between those clicks
	Player *mpActivePawn; //switches after end of each turn. don't worry about null checking this because if it ever equals null then we have bigger problems
	Player *mpSelectedPawn; // Refers to the players whose cards are being displayed in the hand section. Might not necessarily be active pawn if user wants to flip through each player's cards
	int mActivePawnIndex; //lets mpActivePawn switch to next pawn in array
	int mSelectedPawnIndex; //lets mpSelectedPawn switch to next pawn in array

	Unit *mBoardOutline; // just draws an outline between the board and card area, explicitly tracked so that we can check whether mouse clicks fall inside its bounds
	Unit* mpMouseHighlightedUnit;

	std::string mStartingCity;

	Color mActiveCardColor;

	int mMovesRemaining, mMaxMovesPerTurn, mNumPlayerCardsToDraw;
	int mNumEpidemicsHad, mOutbreakThreshold;
	std::vector<int> mInfectionsAfterEpidemic;

	PlayerCardDeck *mPlayerDiscardDeck, *mPlayerDrawDeck;
	InfectionCardDeck *mInfectionDiscardDeck, *mInfectionDrawDeck;

	std::map<const CityType, DiseaseStages> mDiseaseStages; // stores info on whether disease at given index is spreading, cured, or eradicated. Will correspond to mDiseaseCubesRemainingByType
	std::map<const CityType, int> mDiseaseCubesRemainingByType; // stores total number of cubes remaining per each city "color" or type. typically, 4 types
	// TODO: store total amount of disease cubes by type and "pull" from it each time one is allocated. If would go negative, lose the game
	std::vector<int> mInitNumCitiesCubes; // stores ratio of disease cubes to dole out for each infection card draw in setup phase

	std::map<std::string, City*> mCities;
	std::vector<Player*> mPlayers;
	std::vector<PlayerInfo*> mPlayerInfos;
};