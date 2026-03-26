#pragma once

#include "ICanvas.hpp"

class ICanvasDrawable
{
public:
	virtual ~ICanvasDrawable() = default;
	virtual void Draw(ICanvas& canvas) const = 0;
};