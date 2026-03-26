#pragma once

#include "Color.hpp"
#include "ICanvasDrawable.hpp"

#include <string>

class IShape : public ICanvasDrawable
{
public:
	virtual double GetArea() = 0;
	virtual double GetPerimeter() = 0;
	virtual std::string ToString() = 0;
	virtual Color GetOutlineColor() const = 0;
};