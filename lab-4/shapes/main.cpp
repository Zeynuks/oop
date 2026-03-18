#include "core/SFMLCanvas.hpp"
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
	ShapeController shapeController;
	const ShapeParser shapeParser(shapeController);
	std::ifstream inputFile("test.txt");

	if (!inputFile.is_open()) {
		std::cerr << "Не удалось открыть файл test.txt" << std::endl;
		return 1;
	}

	shapeParser.Parse(inputFile);

	IShape& shapeWithMaxArea = shapeController.FindMaxAreaShape();
	std::cout << "Max Area shape: " << std::endl;
	std::cout << shapeWithMaxArea.ToString() << std::endl;

	IShape& shapeWithMaxPerimeter = shapeController.FindMaxAreaShape();
	std::cout << "Min Perimeter shape: " << std::endl;
	std::cout << shapeWithMaxPerimeter.ToString() << std::endl;

	SFMLCanvas canvas(WindowSettings::windowTitle, WindowSettings::windowWidth, WindowSettings::windowWidth);

	while (canvas.IsOpen())
	{
		canvas.PollEvents();
		shapeController.DrawShapes(canvas);
		canvas.Display();
	}
}