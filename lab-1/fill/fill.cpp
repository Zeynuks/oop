#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>

enum class AppMode
{
	File,
	Stdin,
	Help,
	Invalid
};

constexpr int GRID_SIZE = 100;
constexpr char WALL = '#';
constexpr char EMPTY = ' ';
constexpr char FILLED = '.';
constexpr char FILL_FLAG = 'O';

struct Point
{
	int y;
	int x;
};

struct Grid
{
	std::array<std::array<char, GRID_SIZE>, GRID_SIZE> data{};
	int height = 0;
	int width = 0;

	std::array<char, GRID_SIZE>& operator[](const size_t y)
	{
		return data[y];
	}

	const std::array<char, GRID_SIZE>& operator[](const size_t y) const
	{
		return data[y];
	}

	Grid()
	{
		Clear();
	}

	void Clear()
	{
		for (auto& row : data)
		{
			row.fill(EMPTY);
		}

		height = 0;
		width = 0;
	}

	char& At(const Point& p)
	{
		return data[p.y][p.x];
	}

	const char& At(const Point& p) const
	{
		return data[p.y][p.x];
	}

	bool IsValid(const Point& p) const
	{
		if (p.y <= 0 || p.y >= height - 1 || p.x <= 0 || p.x >= width - 1)
		{
			return false;
		}
		if (data[p.y][p.x] == WALL || data[p.y][p.x] == FILLED)
		{
			return false;
		}

		return true;
	}
};

Grid ReadGrid(std::istream& input)
{
	Grid grid;

	std::string line;
	while (std::getline(input, line))
	{
		if (grid.height >= GRID_SIZE)
		{
			break;
		}

		int len = std::min(static_cast<int>(line.size()), GRID_SIZE);
		grid.width = std::max(grid.width, len);

		for (int x = 0; x < len; ++x)
		{
			grid[grid.height][x] = line[x];
		}

		++grid.height;
	}
	return grid;
}

void PrintGrid(const Grid& grid, std::ostream& out)
{
	for (int y = 0; y < grid.height; ++y)
	{
		for (int x = 0; x < grid.width; ++x)
		{
			out << grid[y][x];
		}
		out << std::endl;
	}
}

void Fill(Grid& g, const Point start)
{
	if (!g.IsValid(start))
	{
		return;
	}

	std::queue<Point> q;
	q.push(start);

	while (!q.empty())
	{
		Point p = q.front();
		q.pop();

		if (!g.IsValid(p))
		{
			continue;
		}

		if (g.At(p) == EMPTY)
		{
			g.At(p) = FILLED;
		}

		for (int i = 0; i < 4; ++i)
		{
			constexpr int dy[4] = { -1, 1, 0, 0 };
			constexpr int dx[4] = { 0, 0, -1, 1 };
			Point next{ p.y + dy[i], p.x + dx[i] };
			q.push(next);
		}
	}
}

void RunFill(Grid& grid)
{
	for (int y = 0; y < grid.height; ++y)
	{
		for (int x = 0; x < grid.width; ++x)
		{
			if (grid[y][x] == FILL_FLAG)
			{
				grid.height = std::max(grid.height, y + 1);
				grid.width = std::max(grid.width, x + 1);
				Fill(grid, { y, x });
			}
		}
	}
}

AppMode DefineAppMode(const int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		return AppMode::Stdin;
	case 2:
		if (strcmp(argv[1], "-height") == 0 || strcmp(argv[1], "--help") == 0)
		{
			return AppMode::Help;
		}
		break;
	case 3:
		return AppMode::File;
	default:
		break;
	}

	return AppMode::Invalid;
}

void ShowHelpMessage()
{
	std::cout
		<< "Usage:" << std::endl
		<< "  fill <inputFile> <outputFile>" << std::endl
		<< "  fill                # stdin mode" << std::endl
		<< "  fill -height | --help    # show help" << std::endl;
}

void Process(std::istream& in, std::ostream& out)
{
	Grid grid = ReadGrid(in);
	RunFill(grid);
	PrintGrid(grid, out);
}

void ProcessFile(const std::string& inName, const std::string& outName)
{
	std::ifstream in(inName);
	if (!in)
	{
		throw std::runtime_error("Failed to open input file");
	}

	std::ofstream out(outName);
	if (!out)
	{
		throw std::runtime_error("Failed to open output file");
	}

	Process(in, out);
}

int main(const int argc, char* argv[])
{
	const AppMode mode = DefineAppMode(argc, argv);

	try
	{
		switch (mode)
		{
		case AppMode::File:
			ProcessFile(argv[1], argv[2]);
			break;
		case AppMode::Stdin:
			Process(std::cin, std::cout);
			break;
		case AppMode::Help:
			ShowHelpMessage();
			break;
		case AppMode::Invalid:
			throw std::runtime_error("Invalid arguments. Use -height | --help for usage info.");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}