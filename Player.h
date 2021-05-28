#pragma once
#include "PlayerCard.h"
#include "unit.h"
#include <vector>

class City;

class Player : public Unit
{
public:
	Player(City *city, std::vector<PlayerCard*> cards, Sprite *s);

	~Player() {}

	void dealCard(PlayerCard *pc);

	void discardCard(PlayerCard* pc);

	bool isInCity(City* city);

	void moveCity(City* newCity);

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