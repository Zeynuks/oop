#pragma once

#include <memory>
#include <vector>

/**
 * @brief Шаблонный класс IntervalTree для работы с интервалами.
 *
 * Позволяет находить пересечения интервалов и удалять перекрывающиеся.
 *
 * @tparam T Тип интервала. Интервал должен поддерживать методы GetStart(), GetEnd(), Size() и оператор сравнения.
 */
template <typename T>
class IntervalTree
{
public:
	using IntervalCollection = std::vector<T>; /**< Коллекция интервалов */

	/**
	 * @brief Конструктор дерева интервалов.
	 * @param intervals Коллекция интервалов.
	 */
	explicit IntervalTree(const IntervalCollection& intervals);

	/**
	 * @brief Находит все интервалы, пересекающиеся с заданным.
	 * @param interval Интервал для поиска пересечений.
	 * @return Коллекция пересекающихся интервалов.
	 */
	IntervalCollection FindOverlaps(const T& interval);

	/**
	 * @brief Удаляет пересекающиеся интервалы, оставляя только "наиболее длинные".
	 * @param intervals Коллекция интервалов для фильтрации.
	 * @return Коллекция интервалов без перекрытий.
	 */
	IntervalCollection RemoveOverlaps(const IntervalCollection& intervals);

private:
	class Node; /**< Внутренний узел дерева интервалов */
	std::unique_ptr<Node> m_root; /**< Корневой узел */
};

#include "IntervalTree.tpp"