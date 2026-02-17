#include "../ExpandTemplate.hpp"
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>

TEST(ExpandTemplateTest, ThrowsOnEmptyTemplate) {
	const std::unordered_map<std::string, std::string> templates{
        {"KEY", "value"}
    };

    EXPECT_THROW(ExpandTemplate("", templates), std::runtime_error);
}

TEST(ExpandTemplateTest, ReturnsOriginalStringIfNoKeysMatch) {
	const std::unordered_map<std::string, std::string> templates{
        {"KEY", "value"}
    };

	const std::string input = "Hello world";
	const std::string output = ExpandTemplate(input, templates);
    EXPECT_EQ(output, input);
}

TEST(ExpandTemplateTest, ReplacesSingleKey) {
	const std::unordered_map<std::string, std::string> templates{
        {"NAME", "Alice"}
    };

	const std::string input = "Hello NAME!";
	const std::string output = ExpandTemplate(input, templates);
    EXPECT_EQ(output, "Hello Alice!");
}

TEST(ExpandTemplateTest, ReplacesMultipleKeys) {
	const std::unordered_map<std::string, std::string> templates{
        {"NAME", "Alice"},
        {"PLACE", "Wonderland"}
    };

	const std::string input = "NAME went to PLACE";
	const std::string output = ExpandTemplate(input, templates);
    EXPECT_EQ(output, "Alice went to Wonderland");
}

TEST(ExpandTemplateTest, HandlesOverlappingKeys) {
	const std::unordered_map<std::string, std::string> templates{
        {"AB", "X"},
        {"ABC", "Y"}
    };

	const std::string input = "ABC";
	const std::string output = ExpandTemplate(input, templates);

    EXPECT_EQ(output, "Y");
}

TEST(ExpandTemplateTest, HandlesKeysAtStringBoundaries) {
	const std::unordered_map<std::string, std::string> templates{
        {"START", "S"},
        {"END", "E"}
    };

	const std::string input = "START middle END";
	const std::string output = ExpandTemplate(input, templates);
    EXPECT_EQ(output, "S middle E");
}

TEST(ExpandTemplateTest, ConsecutiveKeys) {
	const std::unordered_map<std::string, std::string> templates{
        {"A", "1"},
        {"B", "2"},
        {"C", "3"}
    };

	const std::string input = "ABC";
	const std::string output = ExpandTemplate(input, templates);
    EXPECT_EQ(output, "123");
}

TEST(ExpandTemplateTest, NoRecursiveExpansion) {
	const std::unordered_map<std::string, std::string> templates{
	        {"%USER_NAME%", "Super %USER_NAME% {WEEK_DAY}"},
			{"{WEEK_DAY}", "Friday. {WEEK_DAY}"}
	};

	const std::string input =
		"Hello, %USER_NAME%.  \n"
		"Today is {WEEK_DAY}.";

	const std::string expected =
		"Hello, Super %USER_NAME% {WEEK_DAY}.  \n"
		"Today is Friday. {WEEK_DAY}.";

	EXPECT_EQ(ExpandTemplate(input, templates), expected);
}

TEST(ExpandTemplateTest, EmptyTemplateValues) {
	const std::unordered_map<std::string, std::string> templates{
	        {"%USER_NAME%", ""},
			{"{WEEK_DAY}", ""}
	};

	const std::string input =
		"Hello, %USER_NAME%.  \n"
		"Today is {WEEK_DAY}.";

	const std::string expected =
		"Hello, .  \n"
		"Today is .";

	EXPECT_EQ(ExpandTemplate(input, templates), expected);
}

TEST(ExpandTemplateTest, NormalReplacementExample) {
	const std::unordered_map<std::string, std::string> templates{
	        {"%USER_NAME%", "Ivan Petrov"},
			{"{WEEK_DAY}", "Friday"}
	};

	const std::string input =
		"Hello, %USER_NAME%.  \n"
		"Today is {WEEK_DAY}.";

	const std::string expected =
		"Hello, Ivan Petrov.  \n"
		"Today is Friday.";

	EXPECT_EQ(ExpandTemplate(input, templates), expected);
}
