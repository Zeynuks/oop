#include <benchmark/benchmark.h>

#include <set>
#include <vector>

#include "../prime_generator.h"

static void BM_Base(benchmark::State& state)
{
	const int n = state.range(0);

	for (auto _ : state)
	{
		auto result = GeneratePrimeNumbers_Base(n);
		benchmark::DoNotOptimize(result);
	}
}
BENCHMARK(BM_Base)->Arg(1'000'000);


static void BM_Odd(benchmark::State& state)
{
	const int n = state.range(0);

	for (auto _ : state)
	{
		auto result = GeneratePrimeNumbers_Odd(n);
		benchmark::DoNotOptimize(result);
	}
}
BENCHMARK(BM_Odd)->Arg(1'000'000);


static void BM_NoSet(benchmark::State& state)
{
	const int n = state.range(0);

	for (auto _ : state)
	{
		auto result = GeneratePrimeNumbers_NoSet(n);
		benchmark::DoNotOptimize(result);
	}
}
BENCHMARK(BM_NoSet)->Arg(1'000'000);


static void BM_Optimized(benchmark::State& state)
{
	const int n = state.range(0);

	for (auto _ : state)
	{
		auto result = GeneratePrimeNumbers_Optimized(n);
		benchmark::DoNotOptimize(result);
	}
}
BENCHMARK(BM_Optimized)->Arg(1'000'000);


BENCHMARK_MAIN();
