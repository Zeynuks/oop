#include <gtest/gtest.h>
#include <string>
#include <type_traits>
#include "../MyList.hpp"

TEST(MyListMemberTypeTests, TypedefsCheck) {
	using List = MyList<std::string>;

	static_assert(std::is_same_v<List::value_type, std::string>);
	static_assert(std::is_same_v<List::size_type, std::size_t>);
	static_assert(std::is_same_v<List::difference_type, std::ptrdiff_t>);
	static_assert(std::is_same_v<List::reference, std::string&>);
	static_assert(std::is_same_v<List::const_reference, const std::string&>);
}

TEST(MyListMemberTypeTests, IteratorTraits) {
	using List = MyList<int>;
	using Iter = List::iterator;
	using CIter = List::const_iterator;

	static_assert(std::is_same_v<std::iterator_traits<Iter>::iterator_category, std::bidirectional_iterator_tag>);
	static_assert(std::is_same_v<std::iterator_traits<Iter>::value_type, int>);
	static_assert(std::is_same_v<std::iterator_traits<Iter>::pointer, int*>);
	static_assert(std::is_same_v<std::iterator_traits<Iter>::reference, int&>);

	static_assert(std::is_same_v<std::iterator_traits<CIter>::pointer, const int*>);
	static_assert(std::is_same_v<std::iterator_traits<CIter>::reference, const int&>);
}

TEST(MyListMemberTypeTests, AllocatorTraitsRebinding) {
	using List = MyList<int>;

	EXPECT_TRUE((std::is_base_of_v<std::allocator<int>, List::allocator_type>));
}

TEST(MyListMemberTypeTests, ReverseIteratorTraits) {
	using List = MyList<std::string>;

	static_assert(std::is_same_v<List::reverse_iterator, std::reverse_iterator<List::iterator>>);
	static_assert(std::is_same_v<List::const_reverse_iterator, std::reverse_iterator<List::const_iterator>>);
}