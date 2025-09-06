#include "unitManager.h"
#include "game.h"
#include "graphicsSystem.h"

UnitManager::UnitManager()
{
	gpEventSystem->addListener(EventType::UNIT_ADD_EVENT, this);
	gpEventSystem->addListener(EventType::UNIT_REMOVE_EVENT, this);
}

UnitManager::~UnitManager()
{
	cleanup();
}

void UnitManager::setUnitAnimations(const std::map<std::string, Animation*> &animations)
{
	mUnitAnimations = animations;
}

std::map<std::string, Animation*> &UnitManager::getUnitAnimations()
{
	return mUnitAnimations;
}

void UnitManager::addUnit(Unit *unit)
{
	mUnits.push_back(unit);
}

void UnitManager::clear()
{
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		if(mUnits[i])
		{
			delete mUnits[i];
			mUnits[i] = NULL;
		}
	}

	mUnits.clear();
}

void UnitManager::deleteUnit(int index)
{
	delete mUnits[index];
	mUnits[index] = NULL;
	mUnits.erase(mUnits.begin() + index);
}

Unit *UnitManager::getUnit(int index)
{
	return mUnits[index];
}

void UnitManager::draw(const GraphicsLayer layer)
{
	// naive z layer implementation. If this is too slow, then we can keep a vector of unit indexes that partitions the unit vector into z sections
	// makes for slower unit additions/creations and takes more time to implement which is why this is here now
	// Or just sort by z layer every time we add or remove a unit
	for(int j = 0; j <= MAX_Z_LAYERS; j++)
	{
		for(unsigned int i = 0; i < mUnits.size(); i++)
		{
			if(mUnits[i]->getZLayer() == j && !(mUnits[i]->getIsHidden()) && ((mUnits[i]->getIsGuiLayer() && layer == GraphicsLayer::GUI_VIEW) || (!mUnits[i]->getIsGuiLayer() && layer == GraphicsLayer::BASE_VIEW)))
			{
				mUnits[i]->draw();
			}
		}
	}
}

void UnitManager::update(double timeElapsed)
{
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		mUnits[i]->update(timeElapsed);
	}
}

void UnitManager::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == EventType::UNIT_ADD_EVENT)
	{
		const UnitAddEvent &ev = static_cast<const UnitAddEvent&>(theEvent);
		Unit *unit = ev.getUnit();
		assert(unit);
		addUnit(unit);
	}
	else if(theEvent.getType() == EventType::UNIT_REMOVE_EVENT)
	{
		const UnitRemoveEvent &ev = static_cast<const UnitRemoveEvent&>(theEvent);
		std::vector<int> removables;
		for(int i = mUnits.size() - 1; i >= 0; i--) //walk backwards to easily remove Units
		{
			if(mUnits[i] == ev.getUnit()) //this code for deciding removal belongs in the function calling this one
			{
				delete mUnits[i];
				mUnits.erase(mUnits.begin() + i);
			}
		}
	}
}

void UnitManager::cleanup()
{
	clear();

	for(auto it = mUnitAnimations.begin(); it != mUnitAnimations.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	mUnitAnimations.clear();
}