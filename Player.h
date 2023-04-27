#pragma once
#include "PlayerCard.h"
#include "unit.h"
#include <vector>

class City;
class PlayerCardDeck;

class Player : public Unit
{
	friend class PlayerInfo;
public:
	Player(City *city, std::vector<PlayerCard*> cards, const Vector2D &playerHandLocation, Sprite *s);

	virtual ~Player();

	void dealCard(PlayerCard *pc);

	void discardCard(PlayerCard* pc);

	bool hasCard(PlayerCard *pc) const;

	bool tradeCard(PlayerCard *pc, Player* to);

	void replaceHandIntoDeck(PlayerCardDeck *deck);

	bool isInCity(City* city);

	void moveCity(City* const newCity);

	//getters
	City* getCurrentCity();
	std::vector<PlayerCard*> &getHand();

private:
	void positionCard(int index, PlayerCard* pc);

	City *currentCity; //map of cityID to city maintained by Board

	Vector2D mHandStart, mHandDirection;
	static int playerNum;
	
	std::vector<PlayerCard*> mHand;
};