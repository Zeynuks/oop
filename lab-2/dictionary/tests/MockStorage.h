#pragma once

#include "../storage/IStorage.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockStorage final : public IStorage
{
public:
	MOCK_METHOD(Entry*, Add, (const std::string& word), (override));
	MOCK_METHOD(Entry*, Get, (const std::string& word), (const, override));

	MOCK_METHOD(void, Load, (const std::deque<Translations>& dtos), (override));
	MOCK_METHOD(std::deque<Translations>, Upload, (), (const, override));
};
