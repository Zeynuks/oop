#pragma once

#include <cstddef>

/**
 * @brief Класс Interval представляет числовой интервал [start, end].
 *
 * Используется для определения границ совпадений в тексте и проверки перекрытий.
 */
class Interval
{
public:
	/**
	 * @brief Конструктор с указанием начала и конца интервала.
	 * @param start Начало интервала
	 * @param end Конец интервала
	 */
	Interval(size_t start, size_t end);

	/**
	 * @brief Получить начало интервала.
	 * @return Начальная позиция
	 */
	[[nodiscard]] size_t GetStart() const;

	/**
	 * @brief Получить конец интервала.
	 * @return Конечная позиция
	 */
	[[nodiscard]] size_t GetEnd() const;

	/**
	 * @brief Получить длину интервала.
	 * @return Размер интервала
	 */
	[[nodiscard]] size_t Size() const;

	/**
	 * @brief Проверить пересечение с другим интервалом.
	 * @param other Другой интервал
	 * @return True, если есть пересечение
	 */
	[[nodiscard]] bool OverlapsWith(const Interval& other) const;

	/**
	 * @brief Проверить, включает ли интервал точку.
	 * @param point Точка
	 * @return True, если точка внутри интервала
	 */
	[[nodiscard]] bool OverlapsWith(size_t point) const;

	bool operator<(const Interval& other) const;
	bool operator!=(const Interval& other) const;
	bool operator==(const Interval& other) const;

private:
	size_t m_start; /**< Начало интервала */
	size_t m_end; /**< Конец интервала */
};

#include "Interval.tpp"