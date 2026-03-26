#pragma once

#include "Color.hpp"
#include "IShape.hpp"
#include "ISolidShape.hpp"

#include <format>

class BaseShape : public virtual IShape
{
public:
	explicit BaseShape(const Color outlineColor)
		: m_outlineColor(outlineColor)
	{
	}

	Color GetOutlineColor() const override
	{
		return m_outlineColor;
	}

	std::string ToString() override
	{
		return std::format("\tOutlineColor: {}\n", m_outlineColor);
	}

private:
	Color m_outlineColor;
};

class BaseSolidShape : public virtual BaseShape
	, public virtual ISolidShape
{
public:
	explicit BaseSolidShape(const Color outlineColor, const Color fillColor)
		: BaseShape(outlineColor)
		, m_fillColor(fillColor)
	{
	}

	Color GetFillColor() const override
	{
		return m_fillColor;
	}

	std::string ToString() override
	{
		return std::format("\tFillColor: {}\n", m_fillColor)
			+ BaseShape::ToString();
	}

private:
	Color m_fillColor;
};