#include "City.h"
#include "CameraEvents.h"
#include "ColorManager.h"
#include "game.h"
#include "Player.h"
#include "UnitEvents.h"

City::City(const std::string &name, const int type, const Vector2D &pos, Sprite *s) : Unit(pos, s)
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	rapidjson::Value &c = doc["city"];
	GraphicsBufferManager *graphics = &Game::getInstance()->getGraphicsBufferManager();
	const rapidjson::Value &typeColors = c["infoColorByType"].GetArray();
	const rapidjson::Value &colorObject = typeColors[type];
	const Color color = Color(colorObject["r"].GetInt(), colorObject["g"].GetInt(), colorObject["b"].GetInt());
	const ColorManager &colorManager = *ColorManager::getInstance();
	const int padding = c["infoTextPadding"].GetInt();
	mStartingCubeTextsPosition = Vector2D(pos.getX(), pos.getY() + s->getHeight());

	for(int i = (int)CityType::BLUE; i < (int)CityType::LAST; i++)
	{
		const Color typeColor = Color(typeColors[i]["r"].GetInt(), typeColors[i]["g"].GetInt(), typeColors[i]["b"].GetInt());
		UIBox *cubeText = new UIBox(
			mStartingCubeTextsPosition - Vector2D(30 * (float)i, 0), // TODO: remove hardcoded value
			c["fontSize"].GetInt(),
			typeColor,
			"0",
			&Game::getInstance()->getDefaultFont()); //delete called in this dtor
		cubeText->setIsHidden(true);
		cubeText->setOutline(Outline(colorManager.white, colorManager.white, padding));
		mCubeTexts.insert(std::pair(CityType(i), cubeText));
		setDiseaseCubes(c["initialCubes"].GetInt(), CityType(i));
	}

	mNameText = new UIBox(
		Vector2D(pos.getX() + 3 * s->getWidth() / 4, pos.getY() + s->getHeight()),
		c["fontSize"].GetInt(), 
		color,
		"0",
		&Game::getInstance()->getDefaultFont()); //delete called in this dtor	
	mNameText->setOutline(Outline(colorManager.white, colorManager.white, padding));
	mName = name;
	mNameText->setText(mName);
	mOutbreakThreshold = c["outbreakThreshold"].GetInt();	
	mOutbroke = false;
	mType = CityType(type);

	mRadius = c["radius"].GetInt();

	s->setColor(color);

	//calculate potential positions for pawns
	pawnPositions.push_back(Vector2D(pos.getX(), pos.getY() - s->getHeight()));
	pawnPositions.push_back(Vector2D(pos.getX() + s->getWidth() / 3, pos.getY()));
	pawnPositions.push_back(Vector2D(pos.getX() + 2 * s->getWidth() / 3, pos.getY()));
	pawnPositions.push_back(Vector2D(pos.getX() + s->getWidth(), pos.getY() + s->getHeight() / 3));

	gpEventSystem->addListener(EventType::OUTBREAK_EVENT, this);
	gpEventSystem->addListener(EventType::ZOOM_CAMERA_EVENT, this);
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
	switch(theEvent.getType())
	{
	case EventType::OUTBREAK_EVENT:
		mOutbroke = false;
		break;
	case EventType::ZOOM_CAMERA_EVENT:
	{
		const ZoomCameraEvent &ev = static_cast<const ZoomCameraEvent&>(theEvent);
		const double delta = ev.getDelta();
		for(auto& cubeText : mCubeTexts)
		{
			cubeText.second->adjustScale(delta);
		}
		mNameText->adjustScale(delta);
		break;
	}
	default:
		break;
	}
}

void City::addPlayer(Player *player)
{
	mPlayersHere.push_back(player);
	//set player position off to side of city based on number of players
	gpEventSystem->fireEvent(new UnitMoveEvent(player, pawnPositions.front(), 1500));
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
	for(auto& text : mCubeTexts)
	{
		if(text.second)
		{
			delete text.second;
			text.second = nullptr;
		}
	}
	mCubeTexts.clear();
}

void City::draw()
{
	//draw city sprite and city info in text
	Game::getInstance()->getGraphics().draw(mPosition, *mConstantFrame, mTheta, mScale);
	for(auto& text : mCubeTexts)
	{
		if(!text.second->getIsHidden())
		{
			text.second->draw();
		}
	}
	mNameText->draw();
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
		gpEventSystem->fireEvent(new UnitAddEvent(u));
	}
}

bool City::decrementDiseaseCubes(const int decrement)
{
	return decrementDiseaseCubes(decrement, mType);
}

bool City::decrementDiseaseCubes(const int decrement, const CityType type)
{
	if(mDiseaseCubes[type] == 0)
	{
		return false;
	}
	setDiseaseCubes(mDiseaseCubes[type] - decrement);
	return true;
}

void City::clearAllCubes()
{
	setDiseaseCubes(0);
}

void City::incrementDiseaseCubes(const int increment)
{
	return incrementDiseaseCubes(increment, mType);
}

void City::incrementDiseaseCubes(const int increment, const CityType type)
{
	int prevCubes = getNumberOfDiseaseCubes();
	if(prevCubes + increment > mOutbreakThreshold && !mOutbroke)
	{
		//spread a cube to each neighbor and fire outbreak event
		mOutbroke = true;
		for(auto& v : mNeighbors)
		{
			v->incrementDiseaseCubes(1, mType);
		}
		gpEventSystem->fireEvent(new Event(EventType::OUTBREAK_EVENT));
	}
	else
	{
		setDiseaseCubes(mDiseaseCubes[type] + increment);
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
	int totalCubes = 0;
	for(int i = 0; i < (int)CityType::LAST; i++)
	{
		totalCubes += getNumberOfDiseaseCubes(CityType(i));
	}
	return totalCubes;
}

int City::getNumberOfDiseaseCubes(const CityType type) const
{
	return mDiseaseCubes.at(type);
}

CityType City::getType() const
{
	return mType;
}

void City::setDiseaseCubes(const int cubes)
{
	setDiseaseCubes(cubes, mType);
}

void City::setDiseaseCubes(const int cubes, const CityType type)
{
	mDiseaseCubes[type] = cubes;
	if(mDiseaseCubes[type] > mOutbreakThreshold)
	{
		mDiseaseCubes[type] = mOutbreakThreshold;
	}
	mCubeTexts[type]->setText(std::to_string(mDiseaseCubes[type]));
	int numDifferentDiseaseCubes = 0;
	for(auto& v: mDiseaseCubes)
	{
		if(v.second > 0)
		{
			mCubeTexts[v.first]->setIsHidden(false);
			gpEventSystem->fireEvent(new UnitMoveEvent(mCubeTexts[v.first], mStartingCubeTextsPosition - Vector2D(30 * (float)numDifferentDiseaseCubes, 0), 0));
			numDifferentDiseaseCubes++;
		}
		else
		{
			mCubeTexts[v.first]->setIsHidden(true);
		}
	}
}