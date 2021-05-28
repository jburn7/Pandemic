#pragma once
#include "graphicsBuffer.h"
#include <map>

class GraphicsBufferManager
{
public:
	GraphicsBufferManager();

	~GraphicsBufferManager();


	void addGraphicsBuffer(std::string id, GraphicsBuffer *graphicsBuffer);

	void deleteGraphicsBuffer(std::string id);

	GraphicsBuffer *getGraphicsBuffer(std::string id);


	void clear();

	void cleanup();
protected:
	std::map<std::string, GraphicsBuffer*> mGraphicsBuffers;
};