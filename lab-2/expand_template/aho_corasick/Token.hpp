#pragma once

#include "Emit.hpp"
#include <string>

/**
 * @brief Класс Token представляет элемент токенизации текста.
 *
 * Может быть фрагментом текста (Fragment) или совпадением ключевого слова (Match).
 * Используется вместе с Trie для разбиения текста на токены.
 *
 * @tparam CharType Тип символа (например, char или wchar_t)
 */
template <typename CharType>
class Token
{
public:
	/**
	 * @brief Тип токена
	 */
	enum Type
	{
		Fragment, /**< Обычный фрагмент текста */
		Match /**< Совпадение ключевого слова */
	};

	using StringType = std::basic_string<CharType>; /**< Тип строки символов */
	using EmitType = Emit<CharType>; /**< Тип Emit для совпадений */

	/**
	 * @brief Конструктор для фрагмента текста.
	 * @param fragment Текст фрагмента
	 */
	explicit Token(StringType fragment);

	/**
	 * @brief Конструктор для совпадения ключевого слова.
	 * @param fragment Текст совпадения
	 * @param emit Объект Emit с позицией и ключевым словом
	 */
	Token(StringType fragment, const EmitType& emit);

	/**
	 * @brief Проверка, является ли токен совпадением.
	 * @return true, если токен Match
	 */
	[[nodiscard]] bool IsMatch() const;

	/**
	 * @brief Получить текст токена.
	 * @return Фрагмент текста
	 */
	StringType GetFragment() const;

	/**
	 * @brief Получить объект Emit.
	 * @return Emit с позицией и ключевым словом
	 */
	EmitType GetEmit() const;

private:
	Type m_type; /**< Тип токена */
	StringType m_fragment; /**< Текст фрагмента */
	EmitType m_emit; /**< Объект Emit */
};

#include "Token.tpp"