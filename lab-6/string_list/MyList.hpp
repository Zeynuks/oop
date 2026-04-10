#pragma once

#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T, typename Allocator = std::allocator<T>>
class MyList
{
	struct NodeBase
	{
		NodeBase* prev = nullptr;
		NodeBase* next = nullptr;
	};

	struct Node : NodeBase
	{
		T data;
	};

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

	template <bool IsConst>
	class ListIterator
	{
		friend class MyList;
		using NodeBasePtr = std::conditional_t<IsConst, const NodeBase*, NodeBase*>;
		using NodePtr = std::conditional_t<IsConst, const Node*, Node*>;

		NodeBasePtr m_node;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = std::conditional_t<IsConst, const T*, T*>;
		using reference = std::conditional_t<IsConst, const T&, T&>;

		explicit ListIterator(NodeBasePtr node)
			: m_node(node)
		{
		}

		template <bool OtherConst>
		ListIterator(const ListIterator<OtherConst>& other,
			std::enable_if_t<IsConst && !OtherConst, int> = 0)
			: m_node(other.m_node)
		{
		}

		reference operator*() const
		{
			return static_cast<NodePtr>(m_node)->data;
		}

		pointer operator->() const
		{
			return std::addressof(static_cast<NodePtr>(m_node)->data);
		}

		ListIterator& operator++()
		{
			m_node = m_node->next;
			return *this;
		}

		ListIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		ListIterator& operator--()
		{
			m_node = m_node->prev;
			return *this;
		}

		ListIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		bool operator==(const ListIterator& other) const
		{
			return m_node == other.m_node;
		}

		bool operator!=(const ListIterator& other) const
		{
			return m_node != other.m_node;
		}
	};

	using iterator = ListIterator<false>;
	using const_iterator = ListIterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	explicit MyList(const Allocator& alloc = Allocator())
		: m_alloc(alloc)
	{
		InitSentinel();
	}

	~MyList()
	{
		Clear();
		FreeSentinel();
	}

	MyList(const MyList& other)
		: m_alloc(NodeAllocatorTraits::select_on_container_copy_construction(other.m_alloc))
	{
		InitSentinel();
		for (const auto& item : other)
		{
			PushBack(item);
		}
	}

	MyList& operator=(const MyList& other)
	{
		if (this != &other)
		{
			MyList tmp(other);
			Swap(tmp);
		}

		return *this;
	}

	MyList(MyList&& other) noexcept
		: m_alloc(std::move(other.m_alloc))
		, m_sentinel(other.m_sentinel)
		, m_size(other.m_size)
	{
		other.m_sentinel = nullptr;
		other.m_size = 0;
	}

	MyList& operator=(MyList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			FreeSentinel();

			m_alloc = std::move(other.m_alloc);
			m_sentinel = other.m_sentinel;
			m_size = other.m_size;

			other.m_sentinel = nullptr;
			other.m_size = 0;
		}

		return *this;
	}

	reference Front()
	{
		assert(m_size > 0);
		return *begin();
	}

	const_reference Front() const
	{
		assert(m_size > 0);
		return *begin();
	}

	reference Back()
	{
		assert(m_size > 0);
		return *--end();
	}

	const_reference Back() const
	{
		assert(m_size > 0);
		return *--end();
	}

	size_type Size() const noexcept
	{
		return m_size;
	}

	bool Empty() const noexcept
	{
		return m_size == 0;
	}

	iterator begin() noexcept
	{
		return iterator(m_sentinel->next);
	}

	iterator end() noexcept
	{
		return iterator(m_sentinel);
	}

	const_iterator begin() const noexcept
	{
		return const_iterator(m_sentinel->next);
	}

	const_iterator end() const noexcept
	{
		return const_iterator(m_sentinel);
	}

	template <typename... Args>
	iterator Emplace(const_iterator pos, Args&&... args)
	{
		NodeBase* curr = const_cast<NodeBase*>(pos.m_node);
		Node* newNode = CreateNode(std::forward<Args>(args)...);

		newNode->next = curr;
		newNode->prev = curr->prev;
		curr->prev->next = newNode;
		curr->prev = newNode;

		m_size++;

		return iterator(newNode);
	}

	void PushBack(const T& value)
	{
		Emplace(end(), value);
	}

	void PushFront(const T& value)
	{
		Emplace(begin(), value);
	}

	iterator Erase(const_iterator pos)
	{
		assert(pos.m_node != m_sentinel && "Cannot erase end()");
		NodeBase* node = const_cast<NodeBase*>(pos.m_node);
		NodeBase* nextNode = node->next;

		node->prev->next = node->next;
		node->next->prev = node->prev;

		DestroyNode(static_cast<Node*>(node));
		m_size--;

		return iterator(nextNode);
	}

	void Clear() noexcept
	{
		while (m_size > 0)
		{
			Erase(begin());
		}
	}

	void Swap(MyList& other) noexcept
	{
		std::swap(m_sentinel, other.m_sentinel);
		std::swap(m_size, other.m_size);
		std::swap(m_alloc, other.m_alloc);
	}

private:
	[[no_unique_address]] NodeAllocator m_alloc;
	NodeBase* m_sentinel = nullptr;
	size_type m_size = 0;

	void InitSentinel()
	{
		BaseAllocator ba(m_alloc);
		m_sentinel = BaseAllocatorTraits::allocate(ba, 1);
		m_sentinel->next = m_sentinel;
		m_sentinel->prev = m_sentinel;
	}

	void FreeSentinel()
	{
		if (m_sentinel)
		{
			BaseAllocator ba(m_alloc);
			BaseAllocatorTraits::deallocate(ba, m_sentinel, 1);
			m_sentinel = nullptr;
		}
	}

	template <typename... Args>
	Node* CreateNode(Args&&... args)
	{
		Node* newNode = NodeAllocatorTraits::allocate(m_alloc, 1);
		try
		{
			NodeAllocatorTraits::construct(m_alloc, std::addressof(newNode->data), std::forward<Args>(args)...);
		}
		catch (...)
		{
			NodeAllocatorTraits::deallocate(m_alloc, newNode, 1);
			throw;
		}

		return newNode;
	}

	void DestroyNode(Node* node) noexcept
	{
		NodeAllocatorTraits::destroy(m_alloc, std::addressof(node->data));
		NodeAllocatorTraits::deallocate(m_alloc, node, 1);
	}
};