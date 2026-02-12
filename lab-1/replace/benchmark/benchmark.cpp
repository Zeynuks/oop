#include "../replace_core.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <random>
#include <string>

enum class TestCase
{
	Best,
	Average,
	Worst,
	Pathological
};

static std::string text;
static std::string searchStr;
static std::string replaceStr;

static void PrepareData(const TestCase tc)
{
	constexpr size_t N = 50'000'000;

	text.clear();

	switch (tc)
	{
	case TestCase::Best:
		text.assign(N, 'a');
		searchStr = "xyz";
		replaceStr = "12345";
		break;

	case TestCase::Average: {
		std::mt19937 rng(42);
		std::uniform_int_distribution dist('a', 'z');

		text.resize(N);
		for (auto& c : text)
		{
			c = dist(rng);
		}

		searchStr = "xyz";
		replaceStr = "12345";
		break;
	}

	case TestCase::Worst:
		searchStr = "xyz";
		replaceStr = "12345";

		text.reserve(N);
		while (text.size() < N)
		{
			text += searchStr;
		}
		break;

	case TestCase::Pathological:
		text.assign(N, 'a');
		searchStr = "aaa";
		replaceStr = "b";
		break;
	}
}

template <typename Func>
static void RunBenchmark(benchmark::State& state, Func func, const TestCase tc)
{
	PrepareData(tc);

	for (auto _ : state)
	{
		auto result = func(text, searchStr, replaceStr);
		benchmark::DoNotOptimize(result);
	}
}

static void BM_Find_Best(benchmark::State& state)
{
	RunBenchmark(state, ReplaceString, TestCase::Best);
}

static void BM_Find_Average(benchmark::State& state)
{
	RunBenchmark(state, ReplaceString, TestCase::Average);
}

static void BM_Find_Worst(benchmark::State& state)
{
	RunBenchmark(state, ReplaceString, TestCase::Worst);
}

static void BM_Find_Pathological(benchmark::State& state)
{
	RunBenchmark(state, ReplaceString, TestCase::Pathological);
}

static void BM_Search_Best(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithSearch, TestCase::Best);
}

static void BM_Search_Average(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithSearch, TestCase::Average);
}

static void BM_Search_Worst(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithSearch, TestCase::Worst);
}

static void BM_Search_Pathological(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithSearch, TestCase::Pathological);
}

static void BM_Ranges_Best(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithRanges, TestCase::Best);
}

static void BM_Ranges_Average(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithRanges, TestCase::Average);
}

static void BM_Ranges_Worst(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithRanges, TestCase::Worst);
}

static void BM_Ranges_Pathological(benchmark::State& state)
{
	RunBenchmark(state, ReplaceStringWithRanges, TestCase::Pathological);
}

BENCHMARK(BM_Find_Best);
BENCHMARK(BM_Search_Best);
BENCHMARK(BM_Ranges_Best);

BENCHMARK(BM_Find_Average);
BENCHMARK(BM_Search_Average);
BENCHMARK(BM_Ranges_Average);

BENCHMARK(BM_Find_Worst);
BENCHMARK(BM_Search_Worst);
BENCHMARK(BM_Ranges_Worst);

BENCHMARK(BM_Find_Pathological);
BENCHMARK(BM_Search_Pathological);
BENCHMARK(BM_Ranges_Pathological);

BENCHMARK_MAIN();
