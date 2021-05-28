#include "graphicsBuffer.h"

GraphicsBuffer::GraphicsBuffer()
{
	mWidth = mHeight = 0;
}

GraphicsBuffer::GraphicsBuffer(Color &color, int w, int h)
{
	create(w, h);
	mWidth = w;
	mHeight = h;
}

GraphicsBuffer::GraphicsBuffer(std::string &resource)
{
	mBitmap.loadFromFile(resource);

	mWidth = mBitmap.getSize().x;
	mHeight = mBitmap.getSize().y;
}

GraphicsBuffer::~GraphicsBuffer()
{
}

void GraphicsBuffer::create(int w, int h)
{
	mBitmap.create(w, h);
}

void GraphicsBuffer::setHeight(int h)
{
	mHeight = h;
}

void GraphicsBuffer::setWidth(int w)
{
	mWidth = w;
}

int GraphicsBuffer::getHeight()
{
	return mHeight;
}

int GraphicsBuffer::getWidth()
{
	return mWidth;
}