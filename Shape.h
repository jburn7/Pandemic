#pragma once
#include "Vector2D.h"
#include <vector>

enum ShapeType
{
	RECTANGLE,
	CIRCLE,
	COMPLEX
};

/**
* Stores the "physical" properties of a Unit. width, height, shape (rectangle, circle, complex) etc.
* TODO: can this also store origin and rotation from Sprite?
*/
class Shape
{
	/**
	* vertices must be in sequential cw or ccw order
	* vertices will be ignored unless type is COMPLEX
	* if type is complex and vertices not supplied, type will changed to RECTANGLE
	*/
	Shape(const int width, const int height, const ShapeType type = ShapeType::RECTANGLE, const std::vector<Vector2D> &vertices = {});

	// Setters
	void setHeight(int h);
	void setType(const ShapeType type, const std::vector<Vector2D>& vertices);
	void setWidth(int w);

	// Getters
	int getHeight() const;
	ShapeType getType() const;
	int getWidth() const;

private:
	void setVerticesFromType();

	std::vector<Vector2D> vertices;
	
	int height;
	ShapeType type;
	int width;
};