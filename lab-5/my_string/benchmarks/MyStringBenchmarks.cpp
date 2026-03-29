#include "../MyString.hpp"

#include <benchmark/benchmark.h>
#include <string>

auto LONG_STR = "This is a long string that definitely exceeds the SSO threshold of std::string and triggers heap allocation.";
auto SHORT_STR = "Short";

static void BM_MyString_Ctor(benchmark::State& state)
{
	for (auto _ : state)
	{
		MyString s(LONG_STR);
		benchmark::DoNotOptimize(s);
	}
}
BENCHMARK(BM_MyString_Ctor);

static void BM_MyString_Copy(benchmark::State& state)
{
	const MyString source(LONG_STR);
	for (auto _ : state)
	{
		MyString copy(source);
		benchmark::DoNotOptimize(copy);
	}
}
BENCHMARK(BM_MyString_Copy);

static void BM_MyString_Move(benchmark::State& state)
{
	for (auto _ : state)
	{
		MyString source(LONG_STR);
		MyString moved(std::move(source));
		benchmark::DoNotOptimize(moved);
	}
}
BENCHMARK(BM_MyString_Move);

static void BM_MyString_Append(benchmark::State& state)
{
	for (auto _ : state)
	{
		MyString s("Start: ");
		for (int i = 0; i < 10; ++i)
		{
			s += " data ";
		}
		benchmark::DoNotOptimize(s);
	}
}
BENCHMARK(BM_MyString_Append);

static void BM_StdString_Ctor(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::string s(LONG_STR);
		benchmark::DoNotOptimize(s);
	}
}
BENCHMARK(BM_StdString_Ctor);

static void BM_StdString_Copy(benchmark::State& state)
{
	const std::string source(LONG_STR);
	for (auto _ : state)
	{
		std::string copy(source);
		benchmark::DoNotOptimize(copy);
	}
}
BENCHMARK(BM_StdString_Copy);

static void BM_StdString_Move(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::string source(LONG_STR);
		std::string moved(std::move(source));
		benchmark::DoNotOptimize(moved);
	}
}
BENCHMARK(BM_StdString_Move);

static void BM_StdString_Append(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::string s("Start: ");
		for (int i = 0; i < 10; ++i)
		{
			s += " data ";
		}
		benchmark::DoNotOptimize(s);
	}
}
BENCHMARK(BM_StdString_Append);

BENCHMARK_MAIN();