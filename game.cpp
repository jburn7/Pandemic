#include "game.h"
#include "ColorManager.h"

Game *Game::msInstance = NULL;

//private functions
Game::Game()
{
	mUi = NULL;
	mQuit = mJustReset = false;
	mGamestate = Gamestate::PLAYING;
}

Game::~Game()
{
	cleanup();
}

void Game::restartGame()
{
	//mLevelManager.cleanup();
	mUnitManager.clear();
	//mLevelManager.init();
	mJustReset = true;
}

std::map<std::string, Animation*> Game::loadUnitAnimations(rapidjson::Value &a, int tileSize)
{
	std::map<std::string, Animation*> animations;
	for(auto &v : a.GetArray())
	{
		//create animation with given time, using the graphicsBuffer that spritesheet equals, add spritesheets based on the row number * tileSize and the numTiles * tileSize
		//then add it to a map name spritesheet as the key
		std::vector<Sprite*> sprites;
		int y = v["rowNumber"].GetInt();
		for(int i = 0; i < v["numTiles"].GetInt(); i++)
		{
			Sprite *sprite = new Sprite(*mGraphicsBufferManager.getGraphicsBuffer(v["spritesheet"].GetString()));
			sprite->setSourceLoc(i * tileSize, y * tileSize);
			sprite->setWidth(tileSize);
			sprite->setHeight(tileSize);
			sprites.push_back(sprite);
		}
		Animation *animation = new Animation(sprites);
		animation->setSpeed(v["time"].GetInt());
		animations.insert(std::pair<std::string, Animation*>(v["name"].GetString(), animation));
	}

	return animations;
}

//public functions
Game *Game::getInstance()
{
	assert(msInstance != NULL);
	return msInstance;
}

void Game::initInstance()
{
	msInstance = new Game();
}

void Game::cleanupInstance()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
}

void Game::init(const std::string &jsonPath)
{
	JSONData::init(jsonPath.c_str());
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	ColorManager::init(doc["colors"]);
	int width = doc["window"]["width"].GetInt(), height = doc["window"]["height"].GetInt();
	mGraphics.init(doc, width, height, doc[JSONPATH]["title"].GetString());
	mInputSystem.init();
	mAISystem.init(doc["ai"]["ticksPerPlayerMove"].GetInt());
	
	mUi = new UI;

	const std::string assetsPath = doc[JSONPATH]["assetsPath"].GetString();

	//add all names for graphics buffers, then loop through and add them
	std::vector<std::string> graphicsBuffersNames;
	//http://rapidjson.org/md_doc_tutorial.html#QueryObject 
	rapidjson::Value &component = doc[JSONPATH]["graphicsBuffers"];
	for(auto& v : component.GetArray())
	{
		graphicsBuffersNames.push_back(v.GetString());
	}

	//add all names for animating graphics buffers, loop through and add
	for(unsigned int i = 0; i < graphicsBuffersNames.size(); i++)
	{
		mGraphicsBufferManager.addGraphicsBuffer(graphicsBuffersNames[i], new GraphicsBuffer(assetsPath + graphicsBuffersNames[i]));
	}

	//add all names for plain graphics buffers to be created upon init, then loop through and create them
	std::vector<std::string> createGraphicsBuffersNames;
	//http://rapidjson.org/md_doc_tutorial.html#QueryObject 
	component = doc[JSONPATH]["createGraphicsBuffersWithColor"];
	for(auto& v : component.GetArray())
	{
		createGraphicsBuffersNames.push_back(v.GetString());
	}

	ColorManager *colorManager = ColorManager::getInstance();
	//add all names for animating graphics buffers, loop through and add
	for(unsigned int i = 0; i < createGraphicsBuffersNames.size(); i++)
	{
		mGraphicsBufferManager.addGraphicsBuffer(createGraphicsBuffersNames[i], new GraphicsBuffer(1, 1));
	}

	//http://rapidjson.org/md_doc_tutorial.html#QueryObject 
	component = doc[JSONPATH]["animations"];
	//int tileSize = doc[JSONPATH]["tileSize"].GetInt();
	int tileSize = 0; // tileSize not used until I add animations

	//load animations for mUnitManager
	mUnitManager.setUnitAnimations(loadUnitAnimations(component, tileSize));

	mMovementManager.init();
	//mLevelManager.init();

	// Init camera with json values for bounds/speed
	component = doc[JSONPATH]["camera"];
	mCameraManager.init(
		Vector2D(component["boundsWidth"].GetFloat(), component["boundsHeight"].GetFloat()),
		component["panSpeed"].GetFloat(), 
		component["panAcceleration"].GetFloat(),
		component["zoomAmount"].GetFloat(),
		component["maxZoom"].GetFloat(),
		component["minZoom"].GetFloat()
	);

	// Init UI stuff like fonts and text sizes
	mFont.loadFont(std::string(doc[JSONPATH]["assetsPath"].GetString()) + "\\" + doc[JSONPATH]["uiFont"].GetString(), doc[JSONPATH]["fontSize"].GetInt());
	mUi->setFont(&mFont);

	gpEventSystem->addListener(EventType::QUIT_EVENT, this);

	mBoard.init();
}

void Game::cleanup()
{
	if(mUi)
	{
		delete mUi;
		mUi = NULL;
	}

	mGraphicsBufferManager.cleanup();
	mGraphics.cleanup();
	mInputSystem.cleanup();
	//mLevelManager.cleanup();
	//mCameraManager.cleanup();
	mUnitManager.cleanup();

	ColorManager::cleanup();
	JSONData::cleanup();
}

void Game::complete()
{
	mGamestate = Gamestate::COMPLETE;
}

void Game::loop()
{
	//double timePerFrame = 16.7777777777; //should be accurate enough
	double timePerFrame = 4; //should be accurate enough
	double timeOfLastFrame = 0;
	int frames = 0;
	mClock.start();
	Timer fpsTimer;
	while(!mQuit)
	{
		fpsTimer.start();
		gpEventSystem->dispatchAllEvents();
		processInput();
		mCameraManager.update();
		const double updateTime = mClock.getElapsedTime();
		update(updateTime, (updateTime - timeOfLastFrame) * 1000);
		render();
		frames++;
		double fps = frames / mClock.getElapsedTime() * 1000;
		gpEventSystem->fireEvent(new UpdateFPSEvent(EventType::UPDATE_FPS_EVENT, (int)fps));
		fpsTimer.sleepUntilElapsed(timePerFrame);
		timeOfLastFrame = mClock.getElapsedTime();
	}
	mClock.stop();
}

void Game::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == EventType::QUIT_EVENT)
	{
		mQuit = true;
	}
	else if(theEvent.getType() == EventType::START_EVENT)
	{
		if(mGamestate == Gamestate::COMPLETE)
		{
			restartGame();
		}
		mGamestate = Gamestate::PLAYING;
	}
}

GraphicsBufferManager &Game::getGraphicsBufferManager()
{
	return mGraphicsBufferManager;
}

GraphicsSystem &Game::getGraphics()
{
	return mGraphics;
}

InputSystem &Game::getInputSystem()
{
	return mInputSystem;
}

MovementManager& Game::getMovementManager()
{
	return mMovementManager;
}

UnitManager &Game::getUnitManager()
{
	return mUnitManager;
}

void Game::processInput()
{
	mInputSystem.checkForInput();
	mAISystem.update(mBoard);
}

void Game::update(double timeElapsed, double timeSinceLastUpdate)
{
	if(mGamestate == Gamestate::PLAYING && !mJustReset)
	{
		//mLevelManager.update(timeElapsed);
		mUnitManager.update(timeElapsed);
		mMovementManager.update(timeSinceLastUpdate);
	}
}

void Game::render()
{
	mGraphics.clear();
	mGraphics.update(GraphicsLayer::BASE_VIEW);
	mUnitManager.draw(GraphicsLayer::BASE_VIEW);
	mGraphics.update(GraphicsLayer::GUI_VIEW);
	mUnitManager.draw(GraphicsLayer::GUI_VIEW);
	mUi->draw();
	mGraphics.flip();
	//if(mJustReset)
	//{
	//	mJustReset = false;
	//}
}