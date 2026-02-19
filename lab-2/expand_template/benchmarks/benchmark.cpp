#pragma once
#include "../aho_corasick/Trie.hpp"
#include <benchmark/benchmark.h>
#include <string>
#include <random>

static auto GenerateLinearText(size_t length) -> std::string
{
	std::string text;
	text.reserve(length);
	constexpr char pattern[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr size_t pattern_len = sizeof(pattern) - 1;
	for (size_t i = 0; i < length; ++i)
		text.push_back(pattern[i % pattern_len]);
	return text;
}

static std::string GenerateWorstCaseText(size_t length)
{
	return std::string(length, 'a');
}

static Trie<char> BuildTrie(size_t pattern_count)
{
	Trie<char> trie;
	trie.RemoveOverlaps();
	for (size_t i = 1; i <= pattern_count; ++i)
		trie.Insert(std::string(i, 'a'));
	return trie;
}

static void BM_ParseText_Random(benchmark::State& state)
{
	const size_t text_size = state.range(0);
	static std::string big_text = GenerateLinearText(1 << 24);
	static Trie<char> trie = BuildTrie(50);
	const std::string text = big_text.substr(0, text_size);
	for (auto _ : state)
	{
		auto emits = trie.ParseText(text);
		benchmark::DoNotOptimize(emits);
	}
	state.SetComplexityN(text_size);
}

BENCHMARK(BM_ParseText_Random)
	->RangeMultiplier(4)
	->Range(1 << 16, 1 << 24)
	->Complexity();

static void BM_ParseText_WorstCase(benchmark::State& state)
{
	const size_t text_size = state.range(0);
	static std::string big_text = GenerateLinearText(1 << 24);
	static Trie<char> trie = BuildTrie(100);
	const std::string text = big_text.substr(0, text_size);
	for (auto _ : state)
	{
		auto emits = trie.ParseText(text);
		benchmark::DoNotOptimize(emits);
	}
	state.SetComplexityN(text_size);
}

BENCHMARK(BM_ParseText_WorstCase)
	->RangeMultiplier(4)
	->Range(1 << 16, 1 << 24)
	->Complexity();

BENCHMARK_MAIN();