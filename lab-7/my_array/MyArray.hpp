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

	// конструктор по умолчанию
	constexpr MyArray() noexcept = default;

	// конструктор, принимающий пользовательский аллокатор
	constexpr explicit MyArray(const Allocator& alloc) noexcept
		: m_allocator(alloc)
	{
	}

	// конструктор копирования
	constexpr MyArray(const MyArray& other)
		: m_allocator(AllocatorTrails::select_on_container_copy_construction(other.m_allocator))
	{
		Reserve(other.m_size);
		for (std::size_t i = 0; i < other.m_size; ++i)
		{
			PushBack(other.m_data[i]);
		}
	}

	// перемещающий конструктор
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

	// конструктор, инициализирующий массив элементами из списка инициализации
	constexpr MyArray(std::initializer_list<T> list, const Allocator& alloc = Allocator())
		: m_allocator(alloc)
	{
		Reserve(list.size());
		for (const auto& item : list)
		{
			PushBack(item);
		}
	}

	// оператор присваивания списка инициализации
	constexpr MyArray& operator=(std::initializer_list<T> list)
	{
		Assign(list.begin(), list.end());
		return *this;
	}

	// копирующий оператор присваивания (использует идиому copy-and-swap)
	constexpr MyArray& operator=(const MyArray& other)
	{
		if (this != &other)
		{
			MyArray temp(other);
			Swap(temp);
		}

		return *this;
	}

	// перемещающий оператор присваивания
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

	// оператор трехстороннего сравнения (космический корабль)
	[[nodiscard]] constexpr auto operator<=>(const MyArray& other) const
	{
		return std::lexicographical_compare_three_way(
			begin(), end(),
			other.begin(), other.end());
	}

	// оператор проверки на равенство
	[[nodiscard]] constexpr bool operator==(const MyArray& other) const
	{
		return std::equal(begin(), end(), other.begin(), other.end());
	}

	// оператор доступа к элементу по индексу для чтения и записи
	constexpr T& operator[](std::size_t index)
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	// оператор доступа к элементу по индексу для чтения (константный)
	constexpr const T& operator[](std::size_t index) const
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	// деструктор класса - уничтожает созданные объекты и освобождает память
	constexpr ~MyArray()
	{
		Clear();
		if (m_data)
		{
			AllocatorTrails::deallocate(m_allocator, m_data, m_capacity);
		}
	}

	// безопасный доступ к элементу по индексу с проверкой границ (для записи)
	[[nodiscard]] constexpr T& At(std::size_t index)
	{
		if (index >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}

		return m_data[index];
	}

	// безопасный доступ к элементу по индексу с проверкой границ (для чтения)
	[[nodiscard]] constexpr const T& At(std::size_t index) const
	{
		if (index >= m_size)
		{
			throw std::out_of_range("Index out of range");
		}

		return m_data[index];
	}

	// возвращает ссылку на первый элемент массива
	[[nodiscard]] constexpr T& Front()
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[0];
	}

	// возвращает константную ссылку на первый элемент массива
	[[nodiscard]] constexpr const T& Front() const
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[0];
	}

	// возвращает ссылку на последний элемент массива
	[[nodiscard]] constexpr T& Back()
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[m_size - 1];
	}

	// возвращает константную ссылку на последний элемент массива
	[[nodiscard]] constexpr const T& Back() const
	{
		assert(!Empty() && "Method called on empty array");
		return m_data[m_size - 1];
	}

	// проверяет, пуст ли массив
	[[nodiscard]] constexpr bool Empty() const noexcept
	{
		return m_size == 0;
	}

	// возвращает текущее количество элементов в массиве
	[[nodiscard]] constexpr std::size_t Size() const noexcept
	{
		return m_size;
	}

	// возвращает текущую вместимость массива
	[[nodiscard]] constexpr std::size_t Capacity() const noexcept
	{
		return m_capacity;
	}

	// уничтожает все элементы массива, не освобождая память
	constexpr void Clear() noexcept
	{
		for (std::size_t i = 0; i < m_size; ++i)
		{
			AllocatorTrails::destroy(m_allocator, m_data + i);
		}

		m_size = 0;
	}

	// добавляет копию элемента в конец массива
	constexpr void PushBack(const T& value)
	{
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		AllocatorTrails::construct(m_allocator, m_data + m_size, value);
		m_size++;
	}

	// перемещает элемент в конец массива
	constexpr void PushBack(T&& value)
	{
		if (m_size == m_capacity)
		{
			Reserve(m_capacity == 0 ? 1 : m_capacity * 2);
		}

		AllocatorTrails::construct(m_allocator, m_data + m_size, std::move(value));
		m_size++;
	}

	// конструирует элемент непосредственно в конце массива
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

	// удаляет последний элемент массива
	constexpr void PopBack()
	{
		if (m_size > 0)
		{
			m_size--;
			AllocatorTrails::destroy(m_allocator, m_data + m_size);
		}
	}

	// вставляет элемент по указанной позиции и возвращает итератор на него
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

	// удаляет элемент по указанной позиции и возвращает итератор на следующий
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

	// удаляет диапазон элементов [itFirst, itLast)
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

	// резервирует память для хранения как минимум newCapacity элементов
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

	// освобождает неиспользуемую выделенную память, подгоняя вместимость под размер
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

	// изменяет размер массива до заданного (создавая дефолтные элементы или удаляя лишние)
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

	// изменяет размер массива до заданного, заполняя новые элементы копиями value
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

	// заменяет содержимое массива элементами из диапазона итераторов
	template <std::input_iterator InputIterator>
	constexpr void Assign(InputIterator first, InputIterator last)
	{
		Clear();
		for (; first != last; ++first)
		{
			PushBack(*first);
		}
	}

	// заменяет содержимое массива заданным количеством копий value
	constexpr void Assign(const std::size_t count, const T& value)
	{
		Clear();
		Reserve(count);
		for (std::size_t i = 0; i < count; ++i)
		{
			PushBack(value);
		}
	}

	// обменивает содержимое двух массивов
	constexpr void Swap(MyArray& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
		std::swap(m_allocator, other.m_allocator);
	}

	// возвращает итератор,
	// указывающий на первый элемент
	[[nodiscard]] constexpr Iterator begin() noexcept
	{
		return Iterator(m_data);
	}

	// возвращает итератор,
	// указывающий на позицию после последнего элемента
	[[nodiscard]] constexpr Iterator end() noexcept
	{
		return Iterator(m_data + m_size);
	}

	// возвращает константный итератор,
	// указывающий на первый элемент
	[[nodiscard]] constexpr ConstIterator begin() const noexcept
	{
		return cbegin();
	}

	// возвращает константный итератор,
	// указывающий на позицию после последнего элемента
	[[nodiscard]] constexpr ConstIterator end() const noexcept
	{
		return cend();
	}

	// возвращает константный итератор,
	// указывающий на первый элемент
	[[nodiscard]] constexpr ConstIterator cbegin() const noexcept
	{
		return ConstIterator(m_data);
	}

	// возвращает константный итератор,
	// указывающий на позицию после последнего элемента
	[[nodiscard]] constexpr ConstIterator cend() const noexcept
	{
		return ConstIterator(m_data + m_size);
	}

	// возвращает обратный итератор,
	// указывающий на последний элемент
	[[nodiscard]] constexpr ReverseIterator rbegin() noexcept
	{
		return ReverseIterator(end());
	}

	// возвращает обратный итератор,
	// указывающий на позицию перед первым элементом
	[[nodiscard]] constexpr ReverseIterator rend() noexcept
	{
		return ReverseIterator(begin());
	}

	// возвращает константный обратный итератор,
	// указывающий на последний элемент
	[[nodiscard]] constexpr ConstReverseIterator rbegin() const noexcept
	{
		return crbegin();
	}

	// возвращает константный обратный итератор,
	// указывающий на позицию перед первым элементом
	[[nodiscard]] constexpr ConstReverseIterator rend() const noexcept
	{
		return crend();
	}

	// возвращает константный обратный итератор,
	// указывающий на последний элемент
	[[nodiscard]] constexpr ConstReverseIterator crbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	// возвращает константный обратный итератор,
	// указывающий на позицию перед первым элементом
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