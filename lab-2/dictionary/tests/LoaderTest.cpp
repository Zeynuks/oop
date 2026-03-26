#include "../storage/Loader.h"
#include "gtest/gtest.h"

#include <deque>
#include <filesystem>
#include <fstream>
#include <string>

class LoaderTest : public ::testing::Test
{
protected:
	std::filesystem::path tempDir;

	void SetUp() override
	{
		tempDir = std::filesystem::temp_directory_path() / "loader_test";
		std::filesystem::create_directories(tempDir);
	}

	void TearDown() override
	{
		std::filesystem::remove_all(tempDir);
	}
};

TEST_F(LoaderTest, CreatesFileIfNotExists)
{
	const auto filePath = tempDir / "new_dict.txt";

	const Loader loader(filePath.string());
	const auto data = loader.LoadData();

	EXPECT_TRUE(std::filesystem::exists(filePath));
	EXPECT_TRUE(data.empty());
}

TEST_F(LoaderTest, LoadsExistingData)
{
	const auto filePath = tempDir / "dict.txt";

	std::ofstream(filePath) << "cat кот\n"
							<< "dog собака\n";

	const Loader loader(filePath.string());
	const auto data = loader.LoadData();

	ASSERT_EQ(data.size(), 2);
	EXPECT_EQ(data[0].first, "cat");
	EXPECT_EQ(data[0].second, "кот");
	EXPECT_EQ(data[1].first, "dog");
	EXPECT_EQ(data[1].second, "собака");
}

TEST_F(LoaderTest, ThrowsOnInvalidLine)
{
	const auto filePath = tempDir / "bad.txt";
	std::ofstream(filePath) << "onlyword\n";

	const Loader loader(filePath.string());
	EXPECT_THROW(loader.LoadData(), std::invalid_argument);
}

TEST_F(LoaderTest, SavesDataToFile)
{
	auto filePath = tempDir / "save_test.txt";
	Loader loader(filePath.string());

	std::deque<std::pair<std::string, std::string>> words = {
		{ "cat", "кот" },
		{ "dog", "собака" }
	};

	loader.SaveData(words);

	std::ifstream file(filePath);
	std::string line;
	std::getline(file, line);
	EXPECT_EQ(line, "cat кот");
	std::getline(file, line);
	EXPECT_EQ(line, "dog собака");
	EXPECT_FALSE(std::getline(file, line));
}

TEST_F(LoaderTest, LoadAndSaveRoundTrip)
{
	const auto filePath = tempDir / "roundtrip.txt";
	std::deque<std::pair<std::string, std::string>> words = {
		{ "apple", "яблоко" },
		{ "banana", "банан" }
	};

	const Loader loader(filePath.string());
	loader.SaveData(words);

	const Loader loader2(filePath.string());
	const auto loaded = loader2.LoadData();

	ASSERT_EQ(loaded.size(), words.size());
	for (size_t i = 0; i < words.size(); ++i)
	{
		EXPECT_EQ(loaded[i].first, words[i].first);
		EXPECT_EQ(loaded[i].second, words[i].second);
	}
}
