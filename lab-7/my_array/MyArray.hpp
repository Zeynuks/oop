#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>

template <typename T, typename Allocator = std::allocator<T>>
class MyArray
{
public:
	using Iterator = T*;
	using ConstIterator = const T*;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
	using AllocatorTrails = std::allocator_traits<Allocator>;

	constexpr MyArray() noexcept = default;

	constexpr explicit MyArray(const Allocator& alloc) noexcept
		: m_allocator(alloc)
	{
	}

	constexpr MyArray(const MyArray& other)
		: m_allocator(AllocatorTrails::select_on_container_copy_construction(other.m_allocator))
	{
		Reserve(other.m_size);
		for (std::size_t i = 0; i < other.m_size; ++i)
		{
			PushBack(other.m_data[i]);
		}
	}

	constexpr MyArray(MyArray&& other) noexcept
		: m_data(other.m_data)
		, m_size(other.m_size)
		, m_capacity(other.m_capacity)
		, m_allocator(std::move(other.m_allocator))
	{
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	constexpr MyArray(std::initializer_list<T> list, const Allocator& alloc = Allocator())
		: m_allocator(alloc)
	{
		Reserve(list.size());
		for (const auto& item : list)
		{
			PushBack(item);
		}
	}

	constexpr MyArray& operator=(std::initializer_list<T> list)
	{
		Assign(list.begin(), list.end());
		return *this;
	}

	constexpr MyArray& operator=(const MyArray& other)
	{
		if (this != &other)
		{
			MyArray temp(other);
			Swap(temp);
		}

		return *this;
	}

	constexpr MyArray& operator=(MyArray&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			if (m_data)
			{
				AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
			}

			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_allocator = std::move(other.m_allocator);

			other.m_data = nullptr;
			other.m_size = 0;
			other.m_capacity = 0;
		}

		return *this;
	}

	[[nodiscard]] constexpr auto operator<=>(const MyArray& other) const
	{
		return std::lexicographical_compare_three_way(
			begin(), end(),
			other.begin(), other.end());
	}

	[[nodiscard]] constexpr bool operator==(const MyArray& other) const
	{
		return std::equal(begin(), end(), other.begin(), other.end());
	}

	constexpr T& operator[](std::size_t index)
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	constexpr const T& operator[](std::size_t index) const
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	constexpr ~MyArray()
	{
		Clear();
		if (m_data)
		{
			AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
		}
	}

	[[nodiscard]] constexpr T& At(std::size_t index)
	{
		if (index >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}

		return m_data[index];
	}

	[[nodiscard]] constexpr const T& At(std::size_t index) const
	{
		if (index >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}

		return m_data[index];
	}

	[[nodiscard]] constexpr T& Front()
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[0];
	}

	[[nodiscard]] constexpr const T& Front() const
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[0];
	}

	[[nodiscard]] constexpr T& Back()
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[m_size - 1];
	}

	[[nodiscard]] constexpr const T& Back() const
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[m_size - 1];
	}

	[[nodiscard]] constexpr bool Empty() const noexcept
	{
		return m_size == 0;
	}

	[[nodiscard]] constexpr std::size_t Size() const noexcept
	{
		return m_size;
	}

	[[nodiscard]] constexpr std::size_t Capacity() const noexcept
	{
		return m_capacity;
	}

	constexpr void Clear() noexcept
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			AllocatorTrails::destroy(m_allocator, m_data + i);
		}

		m_size = 0;
	}

	constexpr void PushBack(const T& value)
	{
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		AllocatorTrails::construct(m_allocator, m_data + m_size, value);
		m_size++;
	}

	constexpr void PushBack(T&& value)
	{
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		AllocatorTrails::construct(m_allocator, m_data + m_size, std::move(value));
		m_size++;
	}

	template <typename... Args>
	constexpr T& EmplaceBack(Args&&... args)
	{
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		T* ptr = m_data + m_size;
		AllocatorTrails::construct(m_allocator, ptr, std::forward<Args>(args)...);
		m_size++;

		return *ptr;
	}

	constexpr void PopBack()
	{
		if (m_size > 0)
		{
			m_size--;
			AllocatorTrails::destroy(m_allocator, m_data + m_size);
		}
	}

	[[nodiscard]] constexpr Iterator Insert(Iterator pos, const T& value)
	{
		std::size_t index = pos - begin();
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
			pos = m_data + index;
		}

		if (pos != end())
		{
			AllocatorTrails::construct(m_allocator, m_data + m_size, std::move(m_data[m_size - 1]));
			for (std::size_t i = m_size - 1; i > index; --i)
			{
				m_data[i] = std::move(m_data[i - 1]);
			}

			m_data[index] = value;
		}
		else
		{
			AllocatorTrails::construct(m_allocator, m_data + m_size, value);
		}

		m_size++;
		return m_data + index;
	}

	[[nodiscard]] constexpr Iterator Erase(Iterator pos)
	{
		std::size_t index = pos - begin();
		if (index >= m_size)
		{
			return end();
		}

		for (std::size_t i = index; i < m_size - 1; ++i)
		{
			m_data[i] = std::move(m_data[i + 1]);
		}

		m_size--;
		AllocatorTrails::destroy(m_allocator, m_data + m_size);

		return m_data + index;
	}

	[[nodiscard]] constexpr Iterator Erase(Iterator itFirst, Iterator itLast)
	{
		if (itFirst == itLast)
		{
			return itFirst;
		}

		std::size_t firstIdx = itFirst - begin();
		const std::size_t lastIdx = itLast - begin();
		const std::size_t count = lastIdx - firstIdx;

		for (std::size_t i = lastIdx; i < m_size; ++i)
		{
			m_data[i - count] = std::move(m_data[i]);
		}

		for (std::size_t i = m_size - count; i < m_size; ++i)
		{
			AllocatorTrails::destroy(m_allocator, m_data + i);
		}

		m_size -= count;

		return m_data + firstIdx;
	}

	constexpr void Reserve(std::size_t newCapacity)
	{
		if (newCapacity <= m_capacity)
		{
			return;
		}

		T* newData = AllocatorTrails::allocate(m_allocator, newCapacity);

		for (std::size_t i = 0; i < m_size; ++i)
		{
			AllocatorTrails::construct(m_allocator, newData + i, std::move(m_data[i]));
			AllocatorTrails::destroy(m_allocator, m_data + i);
		}

		if (m_data)
		{
			AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
		}

		m_data = newData;
		m_capacity = newCapacity;
	}

	constexpr void ShrinkToFit()
	{
		if (m_size < m_capacity)
		{
			if (m_size == 0)
			{
				AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
				m_data = nullptr;
				m_capacity = 0;
			}
			else
			{
				T* newData = AllocatorTrails::allocate(m_allocator, m_size);
				for (std::size_t i = 0; i < m_size; ++i)
				{
					AllocatorTrails::construct(m_allocator, newData + i, std::move(m_data[i]));
					AllocatorTrails::destroy(m_allocator, m_data + i);
				}

				AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
				m_data = newData;
				m_capacity = m_size;
			}
		}
	}

	constexpr void Resize(const std::size_t newSize)
	{
		if (newSize < m_size)
		{
			for (std::size_t i = newSize; i < m_size; ++i)
			{
				AllocatorTrails::destroy(m_allocator, m_data + i);
			}
		}
		else if (newSize > m_size)
		{
			Reserve(newSize);
			for (std::size_t i = m_size; i < newSize; ++i)
			{
				AllocatorTrails::construct(m_allocator, m_data + i);
			}
		}
		m_size = newSize;
	}

	constexpr void Resize(const std::size_t newSize, const T& value)
	{
		if (newSize < m_size)
		{
			for (std::size_t i = newSize; i < m_size; ++i)
			{
				AllocatorTrails::destroy(m_allocator, m_data + i);
			}
		}
		else if (newSize > m_size)
		{
			Reserve(newSize);
			for (std::size_t i = m_size; i < newSize; ++i)
			{
				AllocatorTrails::construct(m_allocator, m_data + i, value);
			}
		}

		m_size = newSize;
	}

	template <std::input_iterator InputIterator>
	constexpr void Assign(InputIterator first, InputIterator last)
	{
		Clear();
		for (; first != last; ++first)
		{
			PushBack(*first);
		}
	}

	constexpr void Assign(const std::size_t count, const T& value)
	{
		Clear();
		Reserve(count);
		for (std::size_t i = 0; i < count; ++i)
		{
			PushBack(value);
		}
	}

	constexpr void Swap(MyArray& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
		std::swap(m_allocator, other.m_allocator);
	}

	[[nodiscard]] constexpr Iterator begin() noexcept
	{
		return Iterator(m_data);
	}

	[[nodiscard]] constexpr Iterator end() noexcept
	{
		return Iterator(m_data + m_size);
	}

	[[nodiscard]] constexpr ConstIterator begin() const noexcept
	{
		return cbegin();
	}

	[[nodiscard]] constexpr ConstIterator end() const noexcept
	{
		return cend();
	}

	[[nodiscard]] constexpr ConstIterator cbegin() const noexcept
	{
		return ConstIterator(m_data);
	}

	[[nodiscard]] constexpr ConstIterator cend() const noexcept
	{
		return ConstIterator(m_data + m_size);
	}

	[[nodiscard]] constexpr ReverseIterator rbegin() noexcept
	{
		return ReverseIterator(end());
	}

	[[nodiscard]] constexpr ReverseIterator rend() noexcept
	{
		return ReverseIterator(begin());
	}

	[[nodiscard]] constexpr ConstReverseIterator rbegin() const noexcept
	{
		return crbegin();
	}

	[[nodiscard]] constexpr ConstReverseIterator rend() const noexcept
	{
		return crend();
	}

	[[nodiscard]] constexpr ConstReverseIterator crbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	[[nodiscard]] constexpr ConstReverseIterator crend() const noexcept
	{
		return ConstReverseIterator(begin());
	}

private:
	T* m_data = nullptr;
	std::size_t m_size = 0;
	std::size_t m_capacity = 0;
	[[no_unique_address]] Allocator m_allocator;
};