#pragma once
#include "Operations.hpp"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Абстрактный класс для представления выражения.
 *
 * Все типы выражений должны наследовать этот интерфейс.
 */
class IExpression
{
public:
	virtual ~IExpression() = default;

	/**
	 * @brief Возвращает список идентификаторов операндов.
	 *
	 * @return Вектор строк с именами операндов
	 */
	virtual std::vector<std::string> GetOperands() const = 0;

	/**
	 * @brief Вычисляет значение выражения на основе значений операндов.
	 *
	 * @param operandValues Вектор значений операндов
	 * @return Результат вычисления
	 */
	virtual double Calculate(const std::vector<double>& operandValues) const = 0;
};

/**
 * @brief Класс для числового выражения (константы).
 */
class NumberExpression : public IExpression
{
public:
	/**
	 * @brief Конструктор числа.
	 *
	 * @param value Значение числа
	 */
	explicit NumberExpression(double value);

	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;

private:
	double m_value; ///< Значение числа
};

/**
 * @brief Класс для выражения-переменной.
 */
class VariableExpression : public IExpression
{
public:
	/**
	 * @brief Конструктор переменной.
	 *
	 * @param id Имя переменной
	 */
	explicit VariableExpression(std::string id);

	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;

private:
	std::string m_id; ///< Имя переменной
};

/**
 * @brief Класс для бинарного выражения (операции между двумя операндами).
 */
class BinaryExpression : public IExpression
{
public:
	/**
	 * @brief Конструктор бинарного выражения.
	 *
	 * @param operation Операция (Add, Sub, Mul, Div)
	 * @param left Имя левого операнда
	 * @param right Имя правого операнда
	 */
	BinaryExpression(Operation operation, std::string left, std::string right);

	std::vector<std::string> GetOperands() const override;
	double Calculate(const std::vector<double>& values) const override;

private:
	Operation m_operation; ///< Тип операции
	std::string m_leftId; ///< Имя левого операнда
	std::string m_rightId; ///< Имя правого операнда
};