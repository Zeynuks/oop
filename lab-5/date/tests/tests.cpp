#include <gtest/gtest.h>
#include <sstream>
#include "../MyDate.hpp"

TEST(DateConstructor, EmptyConstructorSuccess) {
	constexpr MyDate date;
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(date.GetMonth(), Month::January);
    EXPECT_EQ(date.GetYear(), 1970);
    EXPECT_EQ(date.GetWeekDay(), WeekDay::Thursday);
}

TEST(DateConstructor, TimestampConstructorSuccess) {
	constexpr MyDate date(365);
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(date.GetMonth(), Month::January);
    EXPECT_EQ(date.GetYear(), 1971);
    EXPECT_EQ(date.GetWeekDay(), WeekDay::Friday);
}

TEST(DateConstructor, MinTimestampSuccess) {
	constexpr MyDate date(0);
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(date.GetMonth(), Month::January);
    EXPECT_EQ(date.GetYear(), 1970);
    EXPECT_EQ(date.GetWeekDay(), WeekDay::Thursday);
}

TEST(DateConstructor, MaxTimestampSuccess) {
	constexpr MyDate date(2932896);
    EXPECT_EQ(date.GetDay(), 31);
    EXPECT_EQ(date.GetMonth(), Month::December);
    EXPECT_EQ(date.GetYear(), 9999);
    EXPECT_EQ(date.GetWeekDay(), WeekDay::Friday);
}

TEST(DateConstructor, TooLargeTimestampFail) {
    EXPECT_THROW(MyDate(2932897), std::out_of_range);
}

TEST(DateConstructor, FullDateConstructorSuccess) {
	constexpr unsigned day = 11;
	constexpr auto month = Month::September;
	constexpr unsigned year = 2001;

	const MyDate date(day, month, year);
    EXPECT_EQ(date.GetDay(), day);
    EXPECT_EQ(date.GetMonth(), month);
    EXPECT_EQ(date.GetYear(), year);
    EXPECT_EQ(date.GetWeekDay(), WeekDay::Tuesday);
}

TEST(DateValidation, MinDaySuccess) {
	const MyDate date(1, Month::July, 2005);
    EXPECT_EQ(date.GetDay(), 1);
}

TEST(DateValidation, TooLowDayFail) {
    EXPECT_THROW(MyDate(0, Month::July, 2005), std::out_of_range);
}

TEST(DateValidation, MaxDaySuccess) {
	const MyDate date(31, Month::December, 2025);
    EXPECT_EQ(date.GetDay(), 31);
}

TEST(DateValidation, TooLargeDayFail) {
    EXPECT_THROW(MyDate(32, Month::July, 2005), std::out_of_range);
}

TEST(DateValidation, TooLargeFebruaryDayFail) {
    EXPECT_THROW(MyDate(30, Month::February, 2005), std::out_of_range);
}

TEST(DateValidation, LeapFebruaryDaySuccess) {
	constexpr MyDate date(29, Month::February, 2020);
    EXPECT_EQ(date.GetDay(), 29);
}

TEST(DateValidation, TooLargeNonLeapFebruaryDayFail) {
    EXPECT_THROW(MyDate(29, Month::February, 2005), std::out_of_range);
}

TEST(DateValidation, MinYearSuccess) {
	const MyDate date(1, Month::July, 1970);
    EXPECT_EQ(date.GetYear(), 1970);
}

TEST(DateValidation, TooLowYearFail) {
    EXPECT_THROW(MyDate(1, Month::July, 1969), std::out_of_range);
}

TEST(DateValidation, MaxYearSuccess) {
	const MyDate date(31, Month::December, 9999);
    EXPECT_EQ(date.GetYear(), 9999);
}

TEST(DateValidation, TooLargeYearFail) {
    EXPECT_THROW(MyDate(1, Month::July, 10000), std::out_of_range);
}

TEST(DateArithmetic, PrefixIncrement) {
    MyDate date(1, Month::July, 2005);
	const auto nextDay = ++date;
    EXPECT_EQ(nextDay.GetDay(), 2);
    EXPECT_EQ(date.GetDay(), 2);
}

TEST(DateArithmetic, PostfixIncrement) {
    MyDate date(1, Month::July, 2005);
	const auto currDay = date++;
    EXPECT_EQ(currDay.GetDay(), 1);
    EXPECT_EQ(date.GetDay(), 2);
}

TEST(DateArithmetic, PrefixDecrement) {
    MyDate date(2, Month::July, 2005);
	const auto lastDay = --date;
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(lastDay.GetDay(), 1);
}

TEST(DateArithmetic, PostfixDecrement) {
    MyDate date(2, Month::July, 2005);
	const auto currDay = date--;
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(currDay.GetDay(), 2);
}

TEST(DateAddition, AddDaysSuccess) {
	const MyDate date(1, Month::July, 2005);
	const auto newDate = date + 30;
    EXPECT_EQ(newDate.GetDay(), 31);
}

TEST(DateAddition, AddChangingMonth) {
	const MyDate date(1, Month::July, 2005);
	const auto newDate = date + 31;
    EXPECT_EQ(newDate.GetMonth(), Month::August);
}

TEST(DateAddition, AddChangingYear) {
	const MyDate date(1, Month::December, 2005);
	const auto newDate = date + 31;
    EXPECT_EQ(newDate.GetYear(), 2006);
}

TEST(DateAddition, AddFailAtMax) {
	const MyDate date(31, Month::December, 9999);
	EXPECT_THROW((void)(date + 1), std::out_of_range);
}

TEST(DateSubtraction, SubDaysSuccess) {
	const MyDate date(3, Month::July, 2005);
	const auto newDate = date - 2;
    EXPECT_EQ(newDate.GetDay(), 1);
}

TEST(DateSubtraction, SubDateDiffSuccess) {
	const MyDate date1(1, Month::July, 2005);
	const MyDate date2(5, Month::July, 2005);
    EXPECT_EQ(date2 - date1, 4);
    EXPECT_EQ(date1 - date2, -4);
}

TEST(DateSubtraction, SubChangingMonth) {
	const MyDate date(1, Month::July, 2005);
	const auto newDate = date - 1;
    EXPECT_EQ(newDate.GetMonth(), Month::June);
}

TEST(DateSubtraction, SubFailAtMin) {
	constexpr MyDate date(1, Month::January, 1970);
	EXPECT_THROW((void)(date - 1), std::out_of_range);
}


TEST(DateAssignment, AddAssignSuccess) {
    MyDate date(1, Month::July, 2005);
    date += 30;
    EXPECT_EQ(date.GetDay(), 31);
}

TEST(DateAssignment, SubAssignSuccess) {
    MyDate date(3, Month::July, 2005);
    date -= 2;
    EXPECT_EQ(date.GetDay(), 1);
}

TEST(DateComparison, Equality) {
	const MyDate d1(1, Month::July, 2005);
	const MyDate d2(1, Month::July, 2005);
	const MyDate d3(2, Month::July, 2005);
    
    EXPECT_TRUE(d1 == d2);
    EXPECT_FALSE(d1 == d3);
    EXPECT_TRUE(d1 != d3);
}

TEST(DateComparison, Relational) {
	const MyDate d1(1, Month::July, 2005);
	const MyDate d2(2, Month::July, 2005);
    
    EXPECT_LT(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_LE(d1, d2);
    EXPECT_GE(d2, d1);
}

TEST(DateIO, OutputFormat) {
	const MyDate d1(1, Month::July, 2005);
    std::ostringstream oss;
    oss << d1;
    EXPECT_EQ(oss.str(), "01.07.2005");
}

TEST(DateIO, InputValid) {
    MyDate date;
    std::istringstream iss("01.07.2005");
    iss >> date;
    EXPECT_FALSE(iss.fail());
    EXPECT_EQ(date.GetDay(), 1);
    EXPECT_EQ(date.GetMonth(), Month::July);
}

TEST(DateIO, InputInvalidFormat) {
    MyDate date;
    std::istringstream iss("01-07-2005");
    iss >> date;
    EXPECT_TRUE(iss.fail());
}