#include "City.h"
#include "game.h"
#include "Player.h"

City::City(const std::string &name, const int type, const Vector2D &pos, Sprite *s) : Unit(pos, s)
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	rapidjson::Value &c = doc["city"];
	GraphicsBufferManager *graphics = &Game::getInstance()->getGraphicsBufferManager();
	const Color color = Color(c["infoColor"]["r"].GetInt(), c["infoColor"]["g"].GetInt(), c["infoColor"]["b"].GetInt());

	mCubeText = new UIBox(
		Vector2D(pos.getX(), pos.getY() + s->getHeight()),
		c["fontSize"].GetInt(),
		Vector2D(0, 0),
		color,
		c["infoTextPadding"].GetFloat(), 
		"0",
		new Sprite(*graphics->getGraphicsBuffer(c["infoBackgroundImg"].GetString())),
		new Sprite(*graphics->getGraphicsBuffer(doc["ui"]["defaultUIPaddingImage"].GetString()))); //deletes called in this dtor, UIBox dtor
	mNameText = new UIBox(
		Vector2D(pos.getX() + 3 * s->getWidth() / 4, pos.getY() + s->getHeight()),
		c["fontSize"].GetInt(), 
		Vector2D(0, 0),
		color,
		c["infoTextPadding"].GetFloat(),
		"0",
		new Sprite(*graphics->getGraphicsBuffer(c["infoBackgroundImg"].GetString())),
		new Sprite(*graphics->getGraphicsBuffer(doc["ui"]["defaultUIPaddingImage"].GetString()))); //deletes called in this dtor, UIBox dtor

	mName = name;
	mNameText->setText(mName);
	mOutbreakThreshold = c["outbreakThreshold"].GetInt();
	setDiseaseCubes(c["initialCubes"].GetInt());
	mOutbroke = false;
	mType = type;

	mRadius = c["radius"].GetInt();

	//calculate potential positions for pawns
	pawnPositions.push_back(Vector2D(pos.getX(), pos.getY() - s->getHeight()));
	pawnPositions.push_back(Vector2D(pos.getX() + s->getWidth() / 3, pos.getY()));
	pawnPositions.push_back(Vector2D(pos.getX() + 2 * s->getWidth() / 3, pos.getY()));
	pawnPositions.push_back(Vector2D(pos.getX() + s->getWidth(), pos.getY() + s->getHeight() / 3));

	gpEventSystem->addListener(OUTBREAK_EVENT, this);
}

City::~City()
{
	cleanup();
}

bool City::contains(Vector2D &loc)
{
	return (getCenter() - loc).getLength() < mRadius;
}

void City::handleEvent(const Event &theEvent)
{
	//clear outbreak flags after an outbreak reaction
	if(theEvent.getType() == OUTBREAK_EVENT)
	{
		mOutbroke = false;
	}
}

void City::addPlayer(Player *player)
{
	mPlayersHere.push_back(player);
	//set player position off to side of city based on number of players
	player->setPosition(pawnPositions.front());
	pawnPositions.erase(pawnPositions.begin());

	//handle movement effects (eg medic clearing cubes on touch after disease has been cured)
}

void City::removePlayer(Player *player)
{
	mPlayersHere.erase(std::remove(mPlayersHere.begin(), mPlayersHere.end(), player));
	//free up player's position
	pawnPositions.push_back(player->getPosition());
}

void City::cleanup()
{
	if(mNameText)
	{
		delete mNameText;
		mNameText = nullptr;
	}
	if(mCubeText)
	{
		delete mCubeText;
		mCubeText = nullptr;
	}
}

void City::draw()
{
	//draw city sprite and city info in text
	Game::getInstance()->getGraphics().draw(mPosition, *mConstantFrame, mTheta, mScale);
	mCubeText->draw(Game::getInstance()->getDefaultFont());
	mNameText->draw(Game::getInstance()->getDefaultFont());
}

void City::loadNeighbors(const std::map<std::string, City*> &cities, const std::vector<std::string> &neighbors)
{
	//for each neighbor, set a pointer to it
	//also go ahead and create a constant sprite which will serve as the visual "edge" of the graph, connecting from this city to its neighbor
	for(auto &v : neighbors)
	{
		mNeighbors.push_back(cities.at(v));
		Sprite *s = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("graph_edge.png")); //delete called in unit dtor
		//set width to distance between this and neighbor
		Vector2D p = getCenter();
		Vector2D q = cities.at(v)->getCenter();
		Vector2D pq = p - q;
		double width = pq.getLength();
		s->setWidth((int)width);
		s->setHeight(2);
		//then rotate edge to face neighbor
		double theta = atan((q.getY() - p.getY()) / (q.getX() - p.getX()));
		if(q.getX() < p.getX())
		{
			theta -= PI;
		}
		Unit *u = new Unit(getCenter(), s); //delete called in unit manager dtor
		u->setRotation(theta);
		u->setZLayer(0); // We definitely want these lines to render below everything else in the game, at least until/if we add a background image
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, u));
	}
}

bool City::decrementDiseaseCubes(const int decrement)
{
	mDiseaseCubes;
	if(mDiseaseCubes == 0)
	{
		return false;
	}
	setDiseaseCubes(mDiseaseCubes - decrement);
	return true;
}

void City::clearAllCubes()
{
	setDiseaseCubes(0);
}

void City::incrementDiseaseCubes(const int increment)
{
	int prevCubes = mDiseaseCubes;
	setDiseaseCubes(mDiseaseCubes + increment);
	if(prevCubes + increment > mOutbreakThreshold && !mOutbroke)
	{
		//spread a cube to each neighbor and fire outbreak event
		mOutbroke = true;
		for(auto &v : mNeighbors)
		{
			v->incrementDiseaseCubes(1);	
		}
		gpEventSystem->fireEvent(new Event(OUTBREAK_EVENT));
	}
}

std::string City::getName()
{
	return mName;
}

std::vector<City*> City::getNeighbors()
{
	return mNeighbors;
}

int City::getNumberOfDiseaseCubes() const
{
	return mDiseaseCubes;
}

int City::getType() const
{
	return mType;
}

void City::setDiseaseCubes(const int cubes)
{
	mDiseaseCubes = cubes;
	if(mDiseaseCubes > mOutbreakThreshold)
	{
		mDiseaseCubes = mOutbreakThreshold;
	}
	mCubeText->setText(std::to_string(mDiseaseCubes));
}