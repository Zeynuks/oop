#pragma once

#include <stdexcept>
#include <vector>

template <typename T>
class Stack {
public:
	void Push(const T& element) {
		m_data.push_back(element);
	}

	void Pop() {
		if (m_data.empty()) {
			throw std::runtime_error("Pop from empty stack");
		}

		m_data.pop_back();
	}

	T& Top() {
		if (m_data.empty()) {
			throw std::runtime_error("Top from empty stack");
		}

		return m_data.back();
	}

	const T& Top() const {
		if (m_data.empty()) {
			throw std::runtime_error("Top from empty stack");
		}

		return m_data.back();
	}

	[[nodiscard]] bool Empty() const {
		return m_data.empty();
	}

	[[nodiscard]] std::size_t Size() const {
		return m_data.size();
	}

	void Clear() {
		m_data.clear();
	}

private:
	std::vector<T> m_data;
};
