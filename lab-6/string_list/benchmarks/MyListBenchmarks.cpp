#include "../MyList.hpp"

#include <benchmark/benchmark.h>
#include <list>
#include <string>

struct LargeStruct
{
	char data[1024];
	LargeStruct() { data[0] = 'x'; }
};

template <typename Container>
void BM_StdPushBack(benchmark::State& state)
{
	using T = Container::value_type;
	for (auto _ : state)
	{
		Container l;
		for (int i = 0; i < state.range(0); ++i)
		{
			l.push_back(T{});
		}
		benchmark::DoNotOptimize(l);
	}
	state.SetItemsProcessed(state.iterations() * state.range(0));
}

template <typename T>
void BM_MyListPushBack(benchmark::State& state)
{
	for (auto _ : state)
	{
		MyList<T> l;
		for (int i = 0; i < state.range(0); ++i)
		{
			l.PushBack(T{});
		}
		benchmark::DoNotOptimize(l);
	}
	state.SetItemsProcessed(state.iterations() * state.range(0));
}

template <typename Container>
void BM_StdTraversal(benchmark::State& state)
{
	using T = Container::value_type;
	Container l;
	for (int i = 0; i < state.range(0); ++i)
		l.push_back(T{});

	for (auto _ : state)
	{
		for (auto const& val : l)
		{
			benchmark::DoNotOptimize(val);
		}
		benchmark::ClobberMemory();
	}
}

template <typename T>
void BM_MyListTraversal(benchmark::State& state)
{
	MyList<T> l;
	for (int i = 0; i < state.range(0); ++i)
		l.PushBack(T{});

	for (auto _ : state)
	{
		for (auto const& val : l)
		{
			benchmark::DoNotOptimize(val);
		}
		benchmark::ClobberMemory();
	}
}

#define ARGS ->Arg(100)->Arg(1000)->Arg(10000)

BENCHMARK_TEMPLATE(BM_StdPushBack, std::list<int>)->Name("StdList_PushBack_Int") ARGS;
BENCHMARK_TEMPLATE(BM_MyListPushBack, int)->Name("MyList_PushBack_Int") ARGS;

BENCHMARK_TEMPLATE(BM_StdPushBack, std::list<std::string>)->Name("StdList_PushBack_String") ARGS;
BENCHMARK_TEMPLATE(BM_MyListPushBack, std::string)->Name("MyList_PushBack_String") ARGS;

BENCHMARK_TEMPLATE(BM_StdPushBack, std::list<LargeStruct>)->Name("StdList_PushBack_Large") ARGS;
BENCHMARK_TEMPLATE(BM_MyListPushBack, LargeStruct)->Name("MyList_PushBack_Large") ARGS;

BENCHMARK_TEMPLATE(BM_StdTraversal, std::list<std::string>)->Name("StdList_Traversal_String") ARGS;
BENCHMARK_TEMPLATE(BM_MyListTraversal, std::string)->Name("MyList_Traversal_String") ARGS;

BENCHMARK_MAIN();