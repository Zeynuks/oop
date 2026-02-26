#include "../storage/IStorage.h"
#include "../storage/InMemoryStorage.h"
#include "gtest/gtest.h"

#include <deque>
#include <set>

using Translations = std::pair<std::string, std::string>;

TEST(InMemoryStorageTest, AddAndGetWord)
{
	InMemoryStorage storage;

	Entry* e1 = storage.Add("hello");
	ASSERT_NE(e1, nullptr);
	EXPECT_EQ(e1->word, "hello");

	Entry* e2 = storage.Get("hello");
	EXPECT_EQ(e1, e2);

	EXPECT_EQ(storage.Get("unknown"), nullptr);
}

TEST(InMemoryStorageTest, AddTranslations)
{
	InMemoryStorage storage;

	Entry* e1 = storage.Add("cat");
	Entry* e2 = storage.Add("кот");

	e1->translations.insert(e2);
	e2->translations.insert(e1);

	EXPECT_TRUE(e1->translations.contains(e2));
	EXPECT_TRUE(e2->translations.contains(e1));
}

TEST(InMemoryStorageTest, LoadDTOs)
{
	InMemoryStorage storage;

	std::deque<Translations> dtos = {
		{ "dog", "собака" },
		{ "dog", "пёс" },
		{ "cat", "кот" }
	};

	storage.Load(dtos);

	Entry* dog = storage.Get("dog");
	Entry* sobaka = storage.Get("собака");
	Entry* pes = storage.Get("пёс");
	Entry* cat = storage.Get("cat");
	Entry* kot = storage.Get("кот");

	ASSERT_NE(dog, nullptr);
	ASSERT_NE(sobaka, nullptr);
	ASSERT_NE(pes, nullptr);
	ASSERT_NE(cat, nullptr);
	ASSERT_NE(kot, nullptr);

	EXPECT_TRUE(dog->translations.count(sobaka));
	EXPECT_TRUE(dog->translations.count(pes));
	EXPECT_TRUE(sobaka->translations.count(dog));
	EXPECT_TRUE(pes->translations.count(dog));
	EXPECT_TRUE(cat->translations.count(kot));
}

TEST(InMemoryStorageTest, UploadDTOs)
{
	InMemoryStorage storage;

	storage.Load({ { "dog", "собака" }, { "dog", "пёс" }, { "cat", "кот" } });

	std::deque<Translations> uploaded = storage.Upload();

	const std::set dtoSet(uploaded.begin(), uploaded.end());

	EXPECT_TRUE(dtoSet.contains({ "dog", "пёс" }) || dtoSet.contains({ "пёс", "dog" }));
	EXPECT_TRUE(dtoSet.contains({ "dog", "собака" }) || dtoSet.contains({ "собака", "dog" }));
	EXPECT_TRUE(dtoSet.contains({ "cat", "кот" }) || dtoSet.contains({ "кот", "cat" }));

	EXPECT_EQ(dtoSet.size(), 3);
}

TEST(InMemoryStorageTest, EmptyLoadUpload)
{
	InMemoryStorage storage;

	storage.Load({});
	const auto uploaded = storage.Upload();

	EXPECT_TRUE(uploaded.empty());
}