#pragma once

#include "ICanvas.hpp"
#include "ShapeController.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class SFMLCanvas : public ICanvas
{
public:
    SFMLCanvas(std::string title, unsigned width, unsigned height)
        : m_window(sf::VideoMode(width, height), std::move(title))
    {
    }

    bool IsOpen() const
	{
        return m_window.isOpen();
    }

    void Display()
    {
        m_window.display();
        m_window.clear(sf::Color(MapToSFML(m_background)));
    }

    void PollEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }
        }
    }

    void Close()
    {
        m_window.close();
    }

    void SetBackgroundColor(const Color color)
    {
        m_background = color;
    }

    void DrawLine(const Point from, const Point to, const Color outlineColor) override
    {
		const sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(from.x, from.y), sf::Color(MapToSFML(outlineColor))),
            sf::Vertex(sf::Vector2f(to.x, to.y), sf::Color(MapToSFML(outlineColor)))
        };

        m_window.draw(line, 2, sf::Lines);
    }

    void DrawPolygon(const std::vector<Point>& points, const Color outlineColor) override
    {
        RenderPolygon(points, DefaultColors::Transparent, outlineColor);
    }

    void FillPolygon(const std::vector<Point>& points, const Color fillColor) override
    {
        RenderPolygon(points, fillColor, DefaultColors::Transparent);
    }

    void DrawCircle(const Point center, const double radius, const Color outlineColor) override
    {
        RenderCircle(center, radius, DefaultColors::Transparent, outlineColor);
    }

    void FillCircle(const Point center, const double radius, const Color fillColor) override
    {
        RenderCircle(center, radius, fillColor, DefaultColors::Transparent);
    }

private:
	sf::RenderWindow m_window;
	Color m_background = DefaultColors::White;

    void RenderPolygon(const std::vector<Point>& points, const Color fillColor, const Color outlineColor)
    {
    	sf::ConvexShape polygon(points.size());
    	for (size_t i = 0; i < points.size(); ++i)
    	{
    		polygon.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
    	}

    	polygon.setFillColor(MapToSFML(fillColor));
    	polygon.setOutlineColor(MapToSFML(outlineColor));
    	polygon.setOutlineThickness(outlineColor != DefaultColors::Transparent ? 2.f : 0.f);

    	m_window.draw(polygon);
    }

    void RenderCircle(const Point center, const double radius, const Color fillColor, const Color outlineColor)
    {
    	sf::CircleShape circle(radius);
    	circle.setPosition(center.x, center.y);
    	circle.setOrigin(radius, radius);

    	circle.setFillColor(MapToSFML(fillColor));
    	circle.setOutlineColor(MapToSFML(outlineColor));
    	circle.setOutlineThickness(outlineColor != DefaultColors::Transparent ? 2.f : 0.f);

    	m_window.draw(circle);
    }

	static sf::Color MapToSFML(const Color& color) {
    	return sf::Color(
			color.r,
			color.g,
			color.b,
			color.a
		);
    }
};