#pragma once
#include <string>
#include <vector>

/**
 * @brief Интерфейс команды для калькулятора.
 *
 * Команды реализуют методы для парсинга аргументов и выполнения действия.
 */
class ICommand
{
public:
	/**
	 * @brief Разбирает аргументы команды.
	 *
	 * Метод может быть переопределён в наследниках для обработки аргументов.
	 *
	 * @param args Вектор строковых аргументов команды
	 */
	virtual void ParseArgs(const std::vector<std::string>&) {}

	/**
	 * @brief Выполняет действие команды.
	 *
	 * Метод должен быть реализован в наследниках.
	 */
	virtual void Execute() = 0;

	/**
	 * @brief Виртуальный деструктор.
	 */
	virtual ~ICommand() = default;
};