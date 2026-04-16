#pragma once

#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T, typename Allocator = std::allocator<T>>
class MyList
{
	// базовый узел
	struct NodeBase
	{
		NodeBase* prev = nullptr;
		NodeBase* next = nullptr;
	};

	// узел списка, содержащий данные
	struct Node : NodeBase
	{
		T data;

		// конструктор узла с perfect forwarding
		template <typename... Args>
		Node(Args&&... args)
			: data(std::forward<Args>(args)...)
		{
		}
	};

	// аллокаторы для узлов и базовых узлов
	using NodeAllocator = std::allocator_traits<Allocator>::template rebind_alloc<Node>;
	using BaseAllocator = std::allocator_traits<Allocator>::template rebind_alloc<NodeBase>;
	using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;
	using BaseAllocatorTraits = std::allocator_traits<BaseAllocator>;

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;

	// двунаправленный итератор списка
	template <bool IsConst>
	class ListIterator
	{
		friend class MyList;

		using NodeBasePtr = std::conditional_t<IsConst, const NodeBase*, NodeBase*>;
		using NodePtr = std::conditional_t<IsConst, const Node*, Node*>;

		NodeBasePtr m_node = nullptr;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = std::conditional_t<IsConst, const T*, T*>;
		using reference = std::conditional_t<IsConst, const T&, T&>;

		// конструктор итератора
		explicit ListIterator(NodeBasePtr node)
			: m_node(node)
		{
		}

		// неявное преобразование iterator -> const_iterator
		template <bool OtherConst>
			requires(IsConst && !OtherConst)
		ListIterator(const ListIterator<OtherConst>& other)
			: m_node(other.m_node)
		{
		}

		// разыменование итератора
		reference operator*() const
		{
			assert(m_node && "Dereferencing null iterator");
			return static_cast<NodePtr>(m_node)->data;
		}

		// доступ к членам объекта
		pointer operator->() const
		{
			assert(m_node != nullptr && "Accessing member of null iterator");
			return std::addressof(static_cast<NodePtr>(m_node)->data);
		}

		// переход к следующему элементу (префикс)
		ListIterator& operator++()
		{
			m_node = m_node->next;
			return *this;
		}

		// переход к следующему элементу (постфикс)
		ListIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		// переход к предыдущему элементу (префикс)
		ListIterator& operator--()
		{
			m_node = m_node->prev;
			return *this;
		}

		// переход к предыдущему элементу (постфикс)
		ListIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		// сравнение итераторов
		template <bool OtherConst>
		bool operator==(const ListIterator<OtherConst>& other) const
		{
			return m_node == other.m_node;
		}

		template <bool OtherConst>
		bool operator!=(const ListIterator<OtherConst>& other) const
		{
			return !(*this == other);
		}
	};

	using iterator = ListIterator<false>;
	using const_iterator = ListIterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// конструктор по умолчанию с аллокатором
	explicit MyList(const Allocator& alloc = Allocator())
		: m_alloc(alloc)
		, m_base_alloc(m_alloc)
	{
		InitSentinel();
	}

	// деструктор - очищает список и освобождает sentinel
	~MyList()
	{
		if (m_sentinel)
		{
			Clear();
			BaseAllocatorTraits::deallocate(m_base_alloc, m_sentinel, 1);
		}
	}

	// конструктор копирования
	MyList(const MyList& other)
		: m_alloc(NodeAllocatorTraits::select_on_container_copy_construction(other.m_alloc))
		, m_base_alloc(m_alloc)
	{
		InitSentinel();

		for (const auto& item : other)
		{
			PushBack(item);
		}
	}

	// оператор копирующего присваивания (copy-and-swap)
	MyList& operator=(const MyList& other)
	{
		if (this != &other)
		{
			MyList tmp(other);
			Swap(tmp);
		}

		return *this;
	}

	// перемещающий конструктор
	MyList(MyList&& other) noexcept
		: m_alloc(std::move(other.m_alloc))
		, m_base_alloc(std::move(other.m_base_alloc))
		, m_sentinel(other.m_sentinel)
		, m_size(other.m_size)
	{
		other.m_sentinel = nullptr;
		other.m_size = 0;
	}

	// перемещающий оператор присваивания
	MyList& operator=(MyList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			BaseAllocatorTraits::deallocate(m_base_alloc, m_sentinel, 1);

			if constexpr (NodeAllocatorTraits::propagate_on_container_move_assignment::value)
			{
				m_alloc = std::move(other.m_alloc);
			}

			m_sentinel = other.m_sentinel;
			m_size = other.m_size;

			other.m_sentinel = nullptr;
			other.m_size = 0;
		}

		return *this;
	}

	// доступ к первому элементу
	reference Front()
	{
		assert(!Empty() && "MyList::Front() called on empty list");
		return *begin();
	}

	// константный доступ к первому элементу
	const_reference Front() const
	{
		assert(!Empty() && "MyList::Front() called on empty list");
		return *begin();
	}

	// доступ к последнему элементу
	reference Back()
	{
		assert(!Empty() && "MyList::Back() called on empty list");
		return *--end();
	}

	// константный доступ к последнему элементу
	const_reference Back() const
	{
		assert(!Empty() && "MyList::Back() called on empty list");
		return *--end();
	}

	// количество элементов
	size_type Size() const noexcept
	{
		return m_size;
	}

	// проверка на пустоту
	bool Empty() const noexcept
	{
		return m_size == 0;
	}

	// оператор трехстороннего сравнения
	[[nodiscard]] std::strong_ordering operator<=>(const MyList& other) const
	{
		return std::lexicographical_compare_three_way(
			begin(), end(),
			other.begin(), other.end());
	}

	// оператор проверки на равенство
	[[nodiscard]] bool operator==(const MyList& other) const
	{
		return std::equal(begin(), end(), other.begin(), other.end());
	}

	// итератор на первый элемент
	iterator begin() noexcept
	{
		assert(m_sentinel && "List not initialized");
		return iterator(m_sentinel->next);
	}

	// итератор на конец (sentinel)
	iterator end() noexcept
	{
		assert(m_sentinel && "List not initialized");
		return iterator(m_sentinel);
	}

	const_iterator begin() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_iterator(m_sentinel->next);
	}

	const_iterator end() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_iterator(m_sentinel);
	}

	const_iterator cbegin() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_iterator(m_sentinel->next);
	}

	const_iterator cend() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_iterator(m_sentinel);
	}

	reverse_iterator rbegin() noexcept
	{
		assert(m_sentinel && "List not initialized");
		return reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		assert(m_sentinel && "List not initialized");
		return reverse_iterator(begin());
	}

	const_reverse_iterator rbegin() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_reverse_iterator(end());
	}

	const_reverse_iterator rend() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crbegin() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crend() const noexcept
	{
		assert(m_sentinel && "List not initialized");
		return const_reverse_iterator(begin());
	}

	// вставка элемента перед pos
	template <typename... Args>
	iterator Emplace(const_iterator pos, Args&&... args)
	{
		NodeBase* curr = const_cast<NodeBase*>(pos.m_node);
		Node* newNode = CreateNode(std::forward<Args>(args)...);

		newNode->next = curr;
		newNode->prev = curr->prev;
		curr->prev->next = newNode;
		curr->prev = newNode;

		++m_size;

		return iterator(newNode);
	}

	// добавление элемента в конец
	void PushBack(const T& value)
	{
		Emplace(end(), value);
	}

	// добавление элемента в начало
	void PushFront(const T& value)
	{
		Emplace(begin(), value);
	}

	void PushBack(T&& value)
	{
		Emplace(end(), std::move(value));
	}

	void PushFront(T&& value)
	{
		Emplace(begin(), std::move(value));
	}

	// удаление элемента по позиции
	iterator Erase(const_iterator pos)
	{
		assert(pos.m_node != m_sentinel && "MyList::Erase(): cannot erase end()");

		NodeBase* node = const_cast<NodeBase*>(pos.m_node);
		NodeBase* nextNode = node->next;

		node->prev->next = node->next;
		node->next->prev = node->prev;

		DestroyNode(static_cast<Node*>(node));
		--m_size;

		return iterator(nextNode);
	}

	// удаляет все элементы списка
	void Clear() noexcept
	{
		assert(m_sentinel && "List not initialized");

		NodeBase* current = m_sentinel->next;

		while (current != m_sentinel)
		{
			NodeBase* next = current->next;
			DestroyNode(static_cast<Node*>(current));
			current = next;
		}

		m_sentinel->next = m_sentinel;
		m_sentinel->prev = m_sentinel;
		m_size = 0;
	}

	// обмен содержимым двух списков
	void Swap(MyList& other) noexcept(NodeAllocatorTraits::propagate_on_container_swap::value || std::allocator_traits<Allocator>::is_always_equal::value)
	{
		if constexpr (NodeAllocatorTraits::propagate_on_container_swap::value)
		{
			std::swap(m_alloc, other.m_alloc);
		}
		else
		{
			assert(m_alloc == other.m_alloc && "Swapping lists with unequal allocators");
		}

		std::swap(m_sentinel, other.m_sentinel);
		std::swap(m_size, other.m_size);
	}

	// присваивает элементы из диапазона
	template <typename InputIt>
	void Assign(InputIt first, InputIt last)
	{
		Clear();
		for (; first != last; ++first)
		{
			PushBack(*first);
		}
	}

private:
	[[no_unique_address]] NodeAllocator m_alloc;
	[[no_unique_address]] BaseAllocator m_base_alloc;
	NodeBase* m_sentinel = nullptr;
	size_type m_size = 0;

	// инициализация sentinel-узла (циклический список)
	void InitSentinel()
	{
		m_sentinel = BaseAllocatorTraits::allocate(m_base_alloc, 1);
		m_sentinel->next = m_sentinel;
		m_sentinel->prev = m_sentinel;
	}

	// создание узла
	template <typename... Args>
	Node* CreateNode(Args&&... args)
	{
		Node* newNode = NodeAllocatorTraits::allocate(m_alloc, 1);
		try
		{
			NodeAllocatorTraits::construct(m_alloc, newNode, std::forward<Args>(args)...);
		}
		catch (...)
		{
			NodeAllocatorTraits::deallocate(m_alloc, newNode, 1);
			throw;
		}

		return newNode;
	}

	// уничтожение узла
	void DestroyNode(Node* node) noexcept
	{
		NodeAllocatorTraits::destroy(m_alloc, node);
		NodeAllocatorTraits::deallocate(m_alloc, node, 1);
	}
};