#include "graphicsBufferManager.h"

GraphicsBufferManager::GraphicsBufferManager()
{
}

GraphicsBufferManager::~GraphicsBufferManager()
{
	cleanup();
}

void GraphicsBufferManager::addGraphicsBuffer(std::string id, GraphicsBuffer *graphicsBuffer)
{
	mGraphicsBuffers.insert(std::pair<std::string, GraphicsBuffer*>(id, graphicsBuffer));
}

void GraphicsBufferManager::deleteGraphicsBuffer(std::string id)
{
	delete mGraphicsBuffers[id];
	mGraphicsBuffers[id] = NULL;
	mGraphicsBuffers.erase(id);
}

GraphicsBuffer *GraphicsBufferManager::getGraphicsBuffer(std::string id)
{
	return mGraphicsBuffers.find(id)->second;
}

void GraphicsBufferManager::clear()
{
	cleanup();
}

void GraphicsBufferManager::cleanup()
{
	for(auto i : mGraphicsBuffers)
	{
		if(i.second)
		{
			delete i.second;
			i.second = NULL;
		}
	}

	mGraphicsBuffers.clear();
}
