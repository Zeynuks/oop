#pragma once

#include "Interval.hpp"
#include <string>

/**
 * @brief Класс Emit представляет найденное совпадение ключевого слова в тексте.
 *
 * Используется в Trie и IntervalTree для хранения позиции, ключевого слова и индекса.
 *
 * @tparam CharType Тип символа (например, char или wchar_t)
 */
template <typename CharType>
class Emit : public Interval
{
public:
	using StringType = std::basic_string<CharType>; /**< Тип строки символов */

	/**
	 * @brief Конструктор по умолчанию. Интервал считается пустым.
	 */
	Emit();

	/**
	 * @brief Конструктор с указанием интервала, ключевого слова и индекса.
	 * @param start Начальная позиция совпадения
	 * @param end Конечная позиция совпадения
	 * @param keyword Ключевое слово
	 * @param index Индекс ключевого слова
	 */
	Emit(size_t start, size_t end, const StringType& keyword, unsigned index);

	/**
	 * @brief Получить ключевое слово.
	 * @return Строка ключевого слова
	 */
	StringType GetKeyword() const;

	/**
	 * @brief Получить индекс ключевого слова.
	 * @return Индекс
	 */
	[[nodiscard]] unsigned GetIndex() const;

	/**
	 * @brief Проверка, пустой ли интервал.
	 * @return true, если пустой
	 */
	[[nodiscard]] bool IsEmpty() const;

private:
	StringType m_keyword; /**< Ключевое слово */
	unsigned m_index; /**< Индекс ключевого слова */
};

#include "Emit.tpp"