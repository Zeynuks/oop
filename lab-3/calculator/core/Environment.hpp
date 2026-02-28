#pragma once
#include "IExpression.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Класс Environment для хранения переменных и функций калькулятора.
 *
 * Поддерживает объявление и обновление переменных, определение функций
 * с использованием IExpression, вычисление значений и кеширование результатов.
 */
class Environment
{
public:
	/**
	 * @brief Устанавливает значение переменной.
	 *
	 * Если переменная не существует, она создается.
	 *
	 * @param id Имя переменной
	 * @param val Значение переменной
	 */
	void SetVariable(const std::string& id, double val);

	/**
	 * @brief Устанавливает функцию в среду.
	 *
	 * Функция представлена объектом IExpression.
	 *
	 * @param id Имя функции
	 * @param expr Умный указатель на IExpression
	 */
	void SetFunction(const std::string& id, std::shared_ptr<IExpression> expr);

	/**
	 * @brief Очищает кеш вычислений всех функций.
	 */
	void ClearCache();

	/**
	 * @brief Проверяет, существует ли переменная с данным именем.
	 *
	 * @param id Имя переменной
	 * @return true если переменная существует, иначе false
	 */
	bool IsVariable(const std::string& id) const;

	/**
	 * @brief Проверяет, существует ли функция с данным именем.
	 *
	 * @param id Имя функции
	 * @return true если функция существует, иначе false
	 */
	bool IsFunction(const std::string& id) const;

	/**
	 * @brief Проверяет, существует ли идентификатор (переменная или функция).
	 *
	 * @param id Имя идентификатора
	 * @return true если идентификатор существует, иначе false
	 */
	bool Exists(const std::string& id) const;

	/**
	 * @brief Возвращает значение переменной или вычисленное значение функции.
	 *
	 * @param id Имя переменной или функции
	 * @return Значение типа double
	 * @throw std::runtime_error если идентификатор не найден или возникает циклическая зависимость
	 */
	double GetValue(const std::string& id);

	/**
	 * @brief Возвращает все переменные с их значениями.
	 *
	 * @return Константная ссылка на unordered_map с именами и значениями переменных
	 */
	std::unordered_map<std::string, double> GetAllVariables() const;

	/**
	 * @brief Возвращает все функции с их выражениями.
	 *
	 * @return Константная ссылка на unordered_map с именами функций и указателями на IExpression
	 */
	std::unordered_map<std::string, std::shared_ptr<IExpression>> GetAllFunctions() const;

private:
	void FillEvaluationCache(const std::string& rootId);
	bool TryExpandDependencies(std::vector<std::string>& stack, const std::shared_ptr<IExpression>& expr) const;
	void EvaluateAndStore(const std::string& id, const std::shared_ptr<IExpression>& expr);

	std::unordered_map<std::string, double> m_variables; ///< Переменные и их значения
	std::unordered_map<std::string, std::shared_ptr<IExpression>> m_functions; ///< Функции и их выражения
	std::unordered_map<std::string, double> m_cache; ///< Кеш вычисленных значений функций
};