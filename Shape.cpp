#include "Shape.h"

Shape::Shape(const int width, const int height, const ShapeType type, const std::vector<Vector2D> &vertices) : width(width), height(height), type(type)
{
	setVerticesFromType();
}

void Shape::setType(const ShapeType type, const std::vector<Vector2D>& vertices)
{
	this->type = type;
	setVerticesFromType();
}

void Shape::setVerticesFromType()
{
	switch(type)
	{
		case ShapeType::COMPLEX:
		{
			if(vertices.size() == 0)
			{
				vertices.push_back(Vector2D(0, 0));
				vertices.push_back(Vector2D(width, 0));
				vertices.push_back(Vector2D(width, height));
				vertices.push_back(Vector2D(0, height));
			}
			else
			{
				vertices = vertices;
			}
			break;
		}
		default:
		{
			break;
		}
	}
}
