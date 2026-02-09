#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "../core/Dictionary.h"
#include "MockStorage.h"

struct TestEntry : Entry
{
	explicit TestEntry(std::string w)
	{
		word = std::move(w);
	}
};

TEST(DictionaryTest, AddWordCallsStorageAddWithNormalizedWord)
{
	MockStorage storage;
	const Dictionary dict(storage);

	TestEntry entry("hello");

	EXPECT_CALL(storage, Add("hello")).WillOnce(testing::Return(&entry));

	dict.AddWord("  HeLLo  ");
}

TEST(DictionaryTest, AddWordWithTranslationAddsBothWords)
{
	MockStorage storage;
	Dictionary dict(storage);

	TestEntry wordEntry("cat");
	TestEntry transEntry("кот");

	EXPECT_CALL(storage, Get("cat")).WillOnce(testing::Return(nullptr));
	EXPECT_CALL(storage, Add("cat")).WillOnce(testing::Return(&wordEntry));

	EXPECT_CALL(storage, Get("кот")).WillOnce(testing::Return(nullptr));
	EXPECT_CALL(storage, Add("кот")).WillOnce(testing::Return(&transEntry));

	dict.AddWord("cat", "кот");

	EXPECT_TRUE(wordEntry.translations.count(&transEntry));
	EXPECT_TRUE(transEntry.translations.count(&wordEntry));
}

TEST(DictionaryTest, AddWordWithTranslationWhenWordsAlreadyExist)
{
	MockStorage storage;
	Dictionary dict(storage);

	TestEntry wordEntry("dog");
	TestEntry transEntry("собака");

	EXPECT_CALL(storage, Get("dog")).WillOnce(testing::Return(&wordEntry));
	EXPECT_CALL(storage, Get("собака")).WillOnce(testing::Return(&transEntry));

	EXPECT_CALL(storage, Add(testing::_)).Times(0);

	dict.AddWord("dog", "собака");

	EXPECT_TRUE(wordEntry.translations.count(&transEntry));
	EXPECT_TRUE(transEntry.translations.count(&wordEntry));
}

TEST(DictionaryTest, GetTranslationsReturnsCorrectSet)
{
	MockStorage storage;
	Dictionary dict(storage);

	TestEntry dog("dog");
	TestEntry cat("cat");
	TestEntry wolf("wolf");

	dog.translations.insert(&cat);
	dog.translations.insert(&wolf);

	EXPECT_CALL(storage, Get("dog")).WillOnce(testing::Return(&dog));

	auto result = dict.GetTranslations("dog");

	EXPECT_EQ(result.size(), 2);
	EXPECT_TRUE(result.count("cat"));
	EXPECT_TRUE(result.count("wolf"));
}

TEST(DictionaryTest, GetTranslationsThrowsIfWordNotFound)
{
	MockStorage storage;
	const Dictionary dict(storage);

	EXPECT_CALL(storage, Get("unknown")).WillOnce(testing::Return(nullptr));

	EXPECT_THROW(dict.GetTranslations("unknown"), std::invalid_argument);
}

TEST(DictionaryTest, NormalizeEmptyString)
{
	MockStorage storage;
	const Dictionary dict(storage);

	TestEntry empty("");

	EXPECT_CALL(storage, Add("")).WillOnce(testing::Return(&empty));

	dict.AddWord("   ");
}

TEST(DictionaryTest, NormalizeCyrillic)
{
	MockStorage storage;
	const Dictionary dict(storage);

	TestEntry entry("привет");

	EXPECT_CALL(storage, Add("привет")).WillOnce(testing::Return(&entry));

	dict.AddWord("ПрИвЕт");
}

TEST(DictionaryTest, SelfTranslation)
{
	MockStorage storage;
	const Dictionary dict(storage);

	TestEntry entry("word");

	EXPECT_CALL(storage, Get("word")).WillRepeatedly(testing::Return(&entry));

	dict.AddWord("word", "word");

	EXPECT_TRUE(entry.translations.count(&entry));
}
