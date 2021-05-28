#pragma once
#include "Unit.h"

class InfectionCard : public Unit
{
public:
	InfectionCard(int cid)
	{
		cityID = cid;
	}

	~InfectionCard() {}

private:
	int cityID; //map of cityID to city maintained by Board
};