#include "EconomyContext.hpp"
#include <gtest/gtest.h>

class TestActor : public IActor
{
public:
	explicit TestActor(ActorId id, std::string name)
		: m_id(id)
		, m_name(std::move(name))
	{
	}
	ActorId GetId() const override { return m_id; }
	std::string GetName() const override { return m_name; }

private:
	ActorId m_id;
	std::string m_name;
};

class AnotherActor : public IActor
{
public:
	explicit AnotherActor(ActorId id, std::string name)
		: m_id(id)
		, m_name(std::move(name))
	{
	}
	ActorId GetId() const override { return m_id; }
	std::string GetName() const override { return m_name; }

private:
	ActorId m_id;
	std::string m_name;
};

TEST(EconomyContextTests, AddActorAndGetAllActors)
{
	EconomyContext context;
	context.AddActor(std::make_unique<TestActor>(1, "Alice"));
	context.AddActor(std::make_unique<TestActor>(2, "Bob"));

	auto actors = context.GetAllActors();
	ASSERT_EQ(actors.size(), 2);

	std::vector<ActorId> ids;
	for (auto& actor : actors)
		ids.push_back(actor.get().GetId());

	EXPECT_NE(std::find(ids.begin(), ids.end(), 1), ids.end());
	EXPECT_NE(std::find(ids.begin(), ids.end(), 2), ids.end());
}

TEST(EconomyContextTests, GetActorByIdSuccess)
{
	EconomyContext context;
	context.AddActor(std::make_unique<TestActor>(42, "Charlie"));

	auto& actor = context.GetActor<TestActor>(42);
	ASSERT_EQ(actor.GetId(), 42);
	ASSERT_EQ(actor.GetName(), "Charlie");
}

TEST(EconomyContextTests, GetActorThrowsIfIdNotFound)
{
	EconomyContext context;
	context.AddActor(std::make_unique<TestActor>(1, "Alice"));

	EXPECT_THROW(
		context.GetActor<TestActor>(2),
		std::runtime_error);
}

TEST(EconomyContextTests, GetActorThrowsIfWrongType)
{
	EconomyContext context;
	context.AddActor(std::make_unique<TestActor>(1, "Alice"));

	EXPECT_THROW(
		context.GetActor<AnotherActor>(1),
		std::runtime_error);
}

TEST(EconomyContextTests, GetAllActorsReturnsReferences)
{
	EconomyContext context;
	context.AddActor(std::make_unique<TestActor>(10, "Alice"));
	context.AddActor(std::make_unique<TestActor>(20, "Bob"));

	auto actors = context.GetAllActors();

	for (auto& ref : actors)
	{
		auto& actor = ref.get();
		EXPECT_TRUE(actor.GetId() == 10 || actor.GetId() == 20);
		EXPECT_TRUE(actor.GetName() == "Alice" || actor.GetName() == "Bob");
	}
}