#include "Shape.h"

Shape::Shape() : Shape(0, 0)
{
}

Shape::Shape(const int width, const int height, const ShapeType type, const std::vector<Vector2D> &vertices) : mWidth(width), mHeight(height), mType(type)
{
	setVerticesFromType();
}

void Shape::setType(const ShapeType type, const std::vector<Vector2D>& vertices)
{
	mType = type;
	setVerticesFromType();
}

void Shape::setVerticesFromType()
{
	switch(mType)
	{
		case ShapeType::COMPLEX:
		{
			if(mVertices.size() == 0)
			{
				mVertices.push_back(Vector2D(0, 0));
				mVertices.push_back(Vector2D((float)mWidth, 0));
				mVertices.push_back(Vector2D((float)mWidth, (float)mHeight));
				mVertices.push_back(Vector2D(0, (float)mHeight));
			}
			else
			{
				mVertices = {};
			}
			break;
		}
		default:
		{
			mVertices = {};
			break;
		}
	}
}

const ShapeType Shape::getType() const
{
	return mType;
}

const std::vector<Vector2D>& Shape::getVertices() const
{
	return mVertices;
}

const int Shape::getWidth() const
{
	return mWidth;
}

const int Shape::getHeight() const
{
	return mHeight;
}