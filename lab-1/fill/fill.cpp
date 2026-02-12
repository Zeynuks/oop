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
constexpr char TMP = '*';

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

	[[nodiscard]] const char& At(const Point& p) const
	{
		return data[p.y][p.x];
	}

	[[nodiscard]] bool IsValid(const Point& p) const
	{
		if (p.y < 0 || p.y >= height || p.x < 0 || p.x >= width)
		{
			return false;
		}

		if (data[p.y][p.x] == WALL || data[p.y][p.x] == FILLED)
		{
			return false;
		}

		return true;
	}

	[[nodiscard]] bool IsNearBorder(const Point& p) const
	{
		return p.y == 0 || p.y == height - 1 || p.x == 0 || p.x == width - 1;
	}

	void Read(std::istream& input)
	{
		Clear();
		std::string line;

		while (std::getline(input, line))
		{
			if (height >= GRID_SIZE)
			{
				break;
			}

			int len = std::min(static_cast<int>(line.size()), GRID_SIZE);

			for (int x = 0; x < len; ++x)
			{
				data[height][x] = line[x];
			}

			width = std::max(width, len);
			height++;
		}

		for (int y = 0; y < height; ++y)
		{
			for (int x = width; x < GRID_SIZE; ++x)
			{
				data[y][x] = EMPTY;
			}
		}
	}

	void Print(std::ostream& out) const
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				out << data[y][x];
			}
			out << std::endl;
		}
	}
};

void ProcessNeighbors(const Grid& g, std::queue<Point>& q, const Point& p)
{
	for (int i = 0; i < 4; ++i)
	{
		constexpr int dx[4] = { 0, 0, -1, 1 };
		constexpr int dy[4] = { -1, 1, 0, 0 };
		Point neighbor{ p.y + dy[i], p.x + dx[i] };
		if (const char neighborChar = g.At(neighbor); g.IsValid(neighbor) || neighborChar == FILL_FLAG)
		{
			q.push(neighbor);
		}
	}
}

void FloodFill(Grid& g, std::queue<Point>& q, bool& touchBorder)
{
	while (!q.empty())
	{
		Point p = q.front();
		q.pop();

		char& c = g.At(p);

		if (!(c != TMP && c != WALL))
		{
			continue;
		}

		if (c == EMPTY)
		{
			c = TMP;
		}

		if (g.IsNearBorder(p))
		{
			touchBorder = true;
		}

		ProcessNeighbors(g, q, p);
	}
}

void UpdateTmpCells(Grid& g, const char finalChar)
{
	for (int y = 0; y < g.height; ++y)
	{
		for (int x = 0; x < g.width; ++x)
		{
			if (g[y][x] == TMP)
			{
				g[y][x] = finalChar;
			}
		}
	}
}

bool Fill(Grid& g, const Point start)
{
	if (const char startChar = g.At(start); !(startChar == EMPTY || startChar == FILL_FLAG))
	{
		return false;
	}

	std::queue<Point> q;
	q.push(start);

	bool touchBorder = false;

	FloodFill(g, q, touchBorder);

	const char finalChar = touchBorder ? EMPTY : FILLED;
	UpdateTmpCells(g, finalChar);

	return !touchBorder;
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
	Grid grid;
	grid.Read(in);
	RunFill(grid);
	grid.Print(out);
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