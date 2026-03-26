#pragma once

#include <exception>
#include <iostream>
#include <vector>

std::string HtmlDecode(const std::string& text);
std::vector<std::string> ReadData(std::istream& input);
void PrintData(std::ostream& output, const std::vector<std::string>& lines);