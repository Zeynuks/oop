#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

/**
 * @brief Класс State представляет состояние автомата Ахо-Корасика в дереве Trie.
 *
 * Каждое состояние хранит переходы и связанные с ними ключевые слова, а также информацию о неудачах.
 *
 * @tparam CharType Тип символа (например, char или wchar_t)
 */
template <typename CharType>
class State
{
public:
	using Ptr = State*; /**< Указатель на состояние */
	using UniquePtr = std::unique_ptr<State>; /**< Уникальный указатель на состояние */
	using StringType = std::basic_string<CharType>; /**< Тип строки символов */
	using KeyIndex = std::pair<StringType, unsigned>; /**< Пара (ключевое слово, индекс) */
	using EmitCollection = std::set<KeyIndex>; /**< Коллекция совпадений */
	using StateCollection = std::vector<Ptr>; /**< Коллекция состояний */
	using TransitionCollection = std::vector<CharType>; /**< Коллекция переходов */

	/**
	 * @brief Конструктор по умолчанию.
	 * @param depth Глубина состояния в дереве.
	 */
	State();

	/**
	 * @brief Конструктор с заданной глубиной.
	 * @param depth Глубина состояния в дереве.
	 */
	explicit State(size_t depth);

	/**
	 * @brief Получить глубину состояния.
	 * @return Глубина состояния.
	 */
	[[nodiscard]] size_t GetDepth() const;

	/**
	 * @brief Получить следующее состояние по символу.
	 * @param c Символ.
	 * @return Указатель на следующее состояние.
	 */
	Ptr NextState(CharType c) const;

	/**
	 * @brief Получить следующее состояние по символу, игнорируя корень.
	 *
	 * Используется внутри AddState для корректного добавления новых состояний.
	 * @param c Символ
	 * @return Указатель на следующее состояние или nullptr
	 */
	Ptr NextStateIgnoreRoot(CharType c) const;

	/**
	 * @brief Добавить переход к следующему состоянию.
	 * @param c Символ.
	 * @return Указатель на следующее состояние.
	 */
	Ptr AddState(CharType c);

	/**
	 * @brief Добавить ключевое слово в состояние.
	 * @param keyword Ключевое слово.
	 * @param index Индекс ключевого слова.
	 */
	void AddEmit(const StringType& keyword, unsigned index);

	/**
	 * @brief Добавить несколько ключевых слов.
	 * @param emits Коллекция ключевых слов.
	 */
	void AddEmit(const EmitCollection& emits);

	/**
	 * @brief Получить коллекцию ключевых слов.
	 * @return Коллекция ключевых слов.
	 */
	EmitCollection GetEmits() const;

	/**
	 * @brief Получить состояние неудачи.
	 * @return Указатель на состояние неудачи.
	 */
	Ptr Failure() const;

	/**
	 * @brief Установить состояние неудачи.
	 * @param failState Указатель на состояние неудачи.
	 */
	void SetFailure(Ptr failState);

	/**
	 * @brief Получить все состояния-переходы.
	 * @return Коллекция состояний.
	 */
	StateCollection GetStates() const;

	/**
	 * @brief Получить все переходы.
	 * @return Коллекция символов переходов.
	 */
	TransitionCollection GetTransitions() const;

private:
	size_t m_depth; /**< Глубина состояния в дереве */
	Ptr m_root; /**< Указатель на корень (для состояния глубины 0) */
	std::map<CharType, UniquePtr> m_success; /**< Переходы по символам */
	Ptr m_failure; /**< Указатель на состояние неудачи */
	EmitCollection m_emits; /**< Коллекция совпадений */

	/**
	 * @brief Получить следующее состояние по символу с опцией игнорирования корня.
	 * @param c Символ.
	 * @param ignoreRoot Игнорировать ли корень.
	 * @return Указатель на следующее состояние.
	 */
	Ptr NextState(CharType c, bool ignoreRoot) const;
};

#include "State.tpp"