#pragma once

#include "IShape.hpp"

class ISolidShape : public virtual IShape
{
public:
	virtual Color GetFillColor() const = 0;
};