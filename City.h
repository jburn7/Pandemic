#pragma once
#include <vector>
#include "unit.h"
#include "EventListener.h"
#include "uiBox.h"

class Player;

class City : public Unit, public EventListener
{
public:
	City(const std::string &name, const int type, const Vector2D &pos, Sprite *s);
	~City();

	//overriden functions
	bool contains(Vector2D &loc); //overriding because we can just use circle collision instead of bounding box for cities
	virtual void handleEvent(const Event &theEvent);

	void addPlayer(Player *player); //place player pawn at this city, handle any effects on movement
	void removePlayer(Player *player);

	virtual void draw(); //overriding because I need to make sure edges are drawn under city sprites

	void loadNeighbors(const std::map<std::string, City*> &cities, const std::vector<std::string> &neighbors);

	bool decrementDiseaseCubes(const int decrement); //return true if cubes were above 0 beforehand
	void clearAllCubes();
	void incrementDiseaseCubes(const int increment);

	//getters
	std::string getName();

	std::vector<City*> getNeighbors();

	int getNumberOfDiseaseCubes() const;

	int getType() const;

	//setters
	void setDiseaseCubes(const int cubes);
private:
	void cleanup();

	int mRadius; //radius in pixels to help with collision detection. load in with json. this can also be extended to resize sprite if I want to add that functionality

	int mDiseaseCubes;
	int mOutbreakThreshold; //when num cubes hits this, outbreak occurs
	bool mOutbroke; //set to true upon outbreak, clear upon receiving outbreak event (meaning that outbreak has resolved). lets cities avoid chain outbreaks
	int mType; // typically there are 4 city types corresponding with 4 disease types
	std::string mName;

	UIBox *mCubeText;
	UIBox *mNameText;

	std::vector<City*> mNeighbors;
	std::vector<Player*> mPlayersHere;
	std::vector<Vector2D> pawnPositions; //easily keep track of whee exactly pawns should go in relation to city sprite
};