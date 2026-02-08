#include "html_decode.h"

#include <unordered_map>

const std::unordered_map<std::string_view, char> HTML_DECODE_ENTITIES = {
	{ "&quot;", '"' },
	{ "&apos;", '\'' },
	{ "&lt;", '<' },
	{ "&gt;", '>' },
	{ "&amp;", '&' }
};

std::string HtmlDecode(const std::string& text)
{
	std::string decodeLine;
	decodeLine.reserve(text.size());

	size_t lastPos = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (text[i] != '&')
		{
			continue;
		}

		const size_t semicolonPos = text.find(';', i);
		if (semicolonPos == std::string::npos)
		{
			break;
		}

		std::string_view entity(&text[i], semicolonPos - i + 1);
		if (auto it = HTML_DECODE_ENTITIES.find(entity); it != HTML_DECODE_ENTITIES.end())
		{
			decodeLine.append(text.data() + lastPos, i - lastPos);
			decodeLine += it->second;
			i = semicolonPos;
			lastPos = i + 1;
		}
	}

	decodeLine.append(text.data() + lastPos, text.size() - lastPos);
	return decodeLine;
}

std::vector<std::string> ReadData(std::istream& input)
{
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(input, line))
	{
		lines.push_back(std::move(line));
	}
	return lines;
}

void PrintData(std::ostream& output, const std::vector<std::string>& lines)
{
	for (const auto& line : lines)
	{
		output << HtmlDecode(line) << std::endl;
	}
}