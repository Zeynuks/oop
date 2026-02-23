#pragma once
#include "Environment.hpp"

/**
 * @brief Класс Calculator для работы с переменными и функциями.
 *
 * Поддерживает объявление переменных, присвоение значений, определение функций
 * с одной или двумя переменными и вычисление значений.
 */
class Calculator
{
public:
	/**
	 * @brief Структура для представления функции.
	 *
	 * Содержит левый и правый операнды и операцию, которая будет применена между ними.
	 */
	struct Function
	{
		std::string left; ///< Левый операнд функции
		std::string right; ///< Правый операнд функции
		Operation operation = Operation::None; ///< Операция (None, Add, Sub, Mul, Div)
	};

	/**
	 * @brief Объявляет новую переменную с именем id.
	 *
	 * Изначально переменная имеет значение NaN.
	 *
	 * @param id Имя переменной
	 * @throw std::invalid_argument если имя переменной недопустимо или уже существует
	 */
	void DeclareVariable(const std::string& id);

	/**
	 * @brief Присваивает числовое значение переменной.
	 *
	 * @param id Имя переменной
	 * @param value Значение для присвоения
	 * @throw std::runtime_error если переменная не существует
	 */
	void AssignVariable(const std::string& id, double value);

	/**
	 * @brief Присваивает значение одной переменной другой переменной.
	 *
	 * @param id1 Имя переменной, которой присваивается значение
	 * @param id2 Имя переменной, значение которой используется
	 * @throw std::runtime_error если одна из переменных не существует
	 */
	void AssignVariable(const std::string& id1, const std::string& id2);

	/**
	 * @brief Определяет функцию с одним операндом.
	 *
	 * @param id Имя функции
	 * @param operand Имя переменной или другой функции
	 * @throw std::invalid_argument если имя функции недопустимо
	 */
	void DefineFunction(const std::string& id, const std::string& operand);

	/**
	 * @brief Определяет функцию с двумя операндами и операцией.
	 *
	 * @param id Имя функции
	 * @param function Структура Function, содержащая операнды и операцию
	 * @throw std::invalid_argument если имя функции недопустимо
	 */
	void DefineFunction(const std::string& id, const Function& function);

	/**
	 * @brief Возвращает текущее значение переменной или функции.
	 *
	 * @param id Имя переменной или функции
	 * @return Значение типа double
	 * @throw std::runtime_error если переменная или функция не существует
	 */
	double GetValue(const std::string& id);

	/**
	 * @brief Возвращает все объявленные переменные и их значения.
	 *
	 * @return Ссылка на map с именами и значениями переменных
	 */
	const std::unordered_map<std::string, double>& GetAllVariables() const;

	/**
	 * @brief Возвращает все определенные функции с их вычисленными значениями.
	 *
	 * @return map с именами функций и их значениями
	 */
	std::unordered_map<std::string, double> GetAllFunctions();

private:
	Environment m_env; ///< Среда хранения переменных и функций

	/**
	 * @brief Преобразует операцию в внутренний тип Operation.
	 *
	 * @param operation Операция для преобразования
	 * @return Operation Внутренний тип операции
	 */
	static Operation MapOperation(Operation operation);

	/**
	 * @brief Проверяет, является ли имя переменной допустимым.
	 *
	 * @param id Имя переменной
	 * @return true если имя корректно, иначе false
	 */
	static bool IsValidVariable(const std::string& id);

	/**
	 * @brief Проверяет корректность операнда (имя переменной или функции).
	 *
	 * @param op Имя операнда
	 * @return true если операнд корректен, иначе false
	 */
	bool IsOperandValid(const std::string& op) const;
};