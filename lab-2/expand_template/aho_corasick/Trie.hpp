#pragma once

#include "Emit.hpp"
#include "State.hpp"
#include "Token.hpp"

#include <string>
#include <vector>

/**
 * @brief Шаблонный класс Trie для поиска подстрок и токенизации текста.
 *
 * @tparam CharType Тип символа, используемый в строках (например, char или wchar_t).
 */
template <typename CharType>
class Trie
{
public:
	using StringType = std::basic_string<CharType>; /**< Тип строки для ключевых слов и текста. */
	using TokenType = Token<CharType>; /**< Тип токена. */
	using TokenCollection = std::vector<TokenType>; /**< Коллекция токенов. */
	using EmitType = Emit<CharType>; /**< Тип совпадения ключевого слова. */
	using EmitCollection = std::vector<EmitType>; /**< Коллекция совпадений. */
	using StateType = State<CharType>; /**< Тип состояния автомата. */
	using StatePtr = StateType*; /**< Указатель на состояние. */

	/**
	 * @brief Конфигурация Trie для управления поведением поиска.
	 */
	class Config
	{
	public:
		Config()
			: m_allowOverlaps(true)
			, m_onlyWholeWords(false)
			, m_caseInsensitive(false)
		{
		}

		[[nodiscard]] bool IsAllowOverlaps() const { return m_allowOverlaps; }
		void SetAllowOverlaps(const bool value) { m_allowOverlaps = value; }

		[[nodiscard]] bool IsOnlyWholeWords() const { return m_onlyWholeWords; }
		void SetOnlyWholeWords(const bool value) { m_onlyWholeWords = value; }

		[[nodiscard]] bool IsCaseInsensitive() const { return m_caseInsensitive; }
		void SetCaseInsensitive(const bool value) { m_caseInsensitive = value; }

	private:
		bool m_allowOverlaps; /**< Разрешать пересекающиеся совпадения. */
		bool m_onlyWholeWords; /**< Искать только полные слова. */
		bool m_caseInsensitive; /**< Игнорировать регистр символов. */
	};

	/** @brief Конструктор по умолчанию. */
	Trie()
		: Trie(Config())
	{
	}

	/**
	 * @brief Конструктор с заданной конфигурацией.
	 * @param cfg Конфигурация Trie.
	 */
	explicit Trie(const Config& cfg)
		: m_root(std::make_unique<StateType>())
		, m_config(cfg)
		, m_failureConstructed(false)
		, m_numKeywords(0)
	{
	}

	/** @brief Сделать поиск нечувствительным к регистру. */
	Trie& CaseInsensitive();

	/** @brief Запретить пересекающиеся совпадения. */
	Trie& RemoveOverlaps();

	/** @brief Искать только целые слова. */
	Trie& OnlyWholeWords();

	/**
	 * @brief Вставляет ключевое слово в Trie.
	 * @param keyword Ключевое слово для поиска.
	 */
	void Insert(const StringType& keyword);

	/**
	 * @brief Токенизирует текст, разделяя на совпадения и фрагменты.
	 * @param text Входной текст.
	 * @return Коллекция токенов.
	 */
	TokenCollection Tokenize(const StringType& text);

	/**
	 * @brief Выполняет поиск всех совпадений ключевых слов в тексте.
	 * @param text Входной текст.
	 * @return Коллекция совпадений (Emit).
	 */
	EmitCollection ParseText(const StringType& text);

private:
	std::unique_ptr<StateType> m_root; /**< Корневое состояние Trie. */
	Config m_config; /**< Конфигурация Trie. */
	bool m_failureConstructed; /**< Флаг построения переходов по неудаче. */
	unsigned m_numKeywords; /**< Счётчик добавленных ключевых слов. */

	/** @brief Проверяет, построены ли состояния неудачи, и строит при необходимости. */
	void CheckFailureStates();

	/**
	 * @brief Получает следующее состояние по символу.
	 * @param curState Текущее состояние.
	 * @param c Символ.
	 * @return Указатель на следующее состояние.
	 */
	StatePtr GetState(StatePtr curState, CharType c) const;

	/** @brief Строит состояния неудачи для автомата Ахо-Корасика. */
	void ConstructFailureStates();

	/**
	 * @brief Создает токен-фрагмент между совпадениями.
	 * @param emit Текущее совпадение.
	 * @param text Текст.
	 * @param lastPos Позиция конца предыдущего совпадения.
	 * @return Токен-фрагмент.
	 */
	TokenType CreateFragment(const EmitType& emit, const StringType& text, size_t lastPos) const;

	/**
	 * @brief Создает токен для совпадения.
	 * @param emit Совпадение.
	 * @param text Текст.
	 * @return Токен совпадения.
	 */
	TokenType CreateMatch(const EmitType& emit, const StringType& text) const;

	/**
	 * @brief Сохраняет все совпадения из состояния.
	 * @param pos Текущая позиция в тексте.
	 * @param curState Состояние.
	 * @param collectedEmits Коллекция для сохранения.
	 */
	void StoreEmits(size_t pos, StatePtr curState, EmitCollection& collectedEmits) const;

	/**
	 * @brief Удаляет неполные совпадения, если включен режим OnlyWholeWords.
	 * @param text Текст.
	 * @param collectedEmits Коллекция совпадений.
	 */
	void RemovePartialMatches(const StringType& text, EmitCollection& collectedEmits) const;
};

#include "Trie.tpp"