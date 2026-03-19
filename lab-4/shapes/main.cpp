#include "core/SFMLCanvas.hpp"
#include "core/ShapeFunctions.hpp"
#include "core/ShapeParser.hpp"

#include <iostream>
#include <fstream>

namespace WindowSettings
{
constexpr std::string windowTitle = "Shapes";
constexpr unsigned int windowWidth = 800;
constexpr unsigned int windowHeight = 800;
}; // namespace WindowSettings

int main()
{
	const ShapeParser shapeParser;
	std::ifstream inputFile("test.txt");

	if (!inputFile.is_open()) {
		std::cerr << "Не удалось открыть файл test.txt" << std::endl;
		return 1;
	}

	std::vector<std::unique_ptr<IShape>> shapes = shapeParser.Parse(inputFile);

	IShape& shapeWithMaxArea = ShapeFunctions::FindMaxAreaShape(shapes);
	std::cout << "Max Area shape: " << std::endl;
	std::cout << shapeWithMaxArea.ToString() << std::endl;

	IShape& shapeWithMaxPerimeter = ShapeFunctions::FindMaxAreaShape(shapes);
	std::cout << "Min Perimeter shape: " << std::endl;
	std::cout << shapeWithMaxPerimeter.ToString() << std::endl;

	SFMLCanvas canvas(WindowSettings::windowTitle, WindowSettings::windowWidth, WindowSettings::windowWidth);

	while (canvas.IsOpen())
	{
		canvas.PollEvents();
		for (const auto& shape : shapes) {
			if (shape) {
				shape->Draw(canvas);
			}
		}
		canvas.Display();
	}
}