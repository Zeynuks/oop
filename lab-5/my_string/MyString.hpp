#pragma once

#include "MyArray.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <istream>
#include <ostream>
#include <string>

class MyString
{
public:
	using Iterator = MyArray<char>::Iterator;
	using ConstIterator = MyArray<char>::ConstIterator;
	using ReverseIterator = MyArray<char>::ReverseIterator;
	using ConstReverseIterator = MyArray<char>::ConstReverseIterator;

	// конструктор по умолчанию
	MyString()
	{
		m_data.PushBack('\0');
	}

	// конструктор, инициализирующий строку данными строки
	// с завершающим нулевым символом
	MyString(const char* pString)
	{
		if (pString) {
			const size_t len = strlen(pString);
			m_data.Resize(len + 1);
			std::memcpy(&m_data[0], pString, len);
			m_data[len] = '\0';
		} else {
			m_data.PushBack('\0');
		}
	}

	// конструктор, инициализирующий строку данными из
	// символьного массива заданной длины
	MyString(const char* pString, const size_t length)
	{
		if (length == 0) {
			m_data.PushBack('\0');
			return;
		}
		m_data.Resize(length + 1);
		std::memcpy(&m_data[0], pString, length);
		m_data[length] = '\0';
	}

	// конструктор копирования
	MyString(const MyString& other)
		: m_data(other.m_data)
	{
	}

	// перемещающий конструктор
	//  реализуется совместно с перемещающим оператором присваивания
	MyString(MyString&& other) noexcept
		: m_data(std::move(other.m_data))
	{
	}

	// конструктор, инициализирующий строку данными из
	// строки стандартной библиотеки C++
	explicit MyString(std::string const& stlString)
	{
		m_data.Reserve(stlString.length() + 1);
		for (size_t i = 0; i < stlString.length(); ++i)
		{
			m_data.PushBack(stlString[i]);
		}
		m_data.PushBack('\0');
	}

	// копирующий оператор присваивания
	// реализуется совместно с копирующим конструктором
	MyString& operator=(MyString const& other)
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}

		return *this;
	}

	// перемещающий оператор присваивания
	// реализуется совместно с перемещающим конструктором
	MyString& operator=(MyString&& other) noexcept
	{
		if (this != &other)
		{
			m_data = std::move(other.m_data);
			other.m_data.PushBack('\0');
		}

		return *this;
	}

	// оператор трехстороннего сравнения (космический корабль)
	// автоматически генерирует операторы <, <=, >, >=
	[[nodiscard]] std::strong_ordering operator<=>(const MyString& other) const = default;

	// оператор проверки на равенство
	// реализуется отдельно для оптимизации производительности
	[[nodiscard]] bool operator==(const MyString& other) const
	{
		return m_data == other.m_data;
	}

	// оператор добавления строки MyString к текущей
	MyString& operator+=(const MyString& other)
	{
		Append(other.GetStringData(), other.GetLength());
		return *this;
	}

	// оператор добавления стандартной строки std::string к текущей
	MyString& operator+=(const std::string& other)
	{
		Append(other.c_str(), other.length());
		return *this;
	}

	// оператор добавления символьного массива (C-строки) к текущей
	MyString& operator+=(const char* pString)
	{
		if (pString)
		{
			Append(pString, strlen(pString));
		}

		return *this;
	}

	// оператор конкатенации двух строк MyString
	[[nodiscard]] friend MyString operator+(MyString lhs, const MyString& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// оператор конкатенации строки MyString и стандартной строки std::string
	[[nodiscard]] friend MyString operator+(MyString lhs, const std::string& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// оператор конкатенации стандартной строки std::string и строки MyString
	[[nodiscard]] friend MyString operator+(const std::string& lhs, const MyString& rhs)
	{
		MyString result(lhs);
		result += rhs;
		return result;
	}

	// оператор конкатенации строки MyString и символьного массива (C-строки)
	[[nodiscard]] friend MyString operator+(MyString lhs, const char* rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// оператор конкатенации символьного массива (C-строки) и строки MyString
	[[nodiscard]] friend MyString operator+(const char* lhs, const MyString& rhs)
	{
		MyString result(lhs);
		result += rhs;
		return result;
	}

	// оператор доступа к символу по индексу для чтения
	// возвращает копию символа по заданному смещению
	[[nodiscard]] char operator[](const size_t index) const
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	// оператор доступа к символу по индексу для записи
	// возвращает ссылку на символ, позволяя изменять содержимое строки
	[[nodiscard]] char& operator[](const size_t index)
	{
		assert(index <= GetLength() && "Index out of bounds");
		return m_data[index];
	}

	// деструктор класса - освобождает память, занимаемую символами строки
	~MyString()
	{
		m_data.Clear();
	}

	// возвращает длину строки (без учета завершающего нулевого символа)
	[[nodiscard]] constexpr size_t GetLength() const
	{
		return m_data.Size() > 0 ? m_data.Size() - 1 : 0;
	}

	// возвращает указатель на массив символов строки
	// в конце массива обязательно должен быть завершающий нулевой символ
	// даже если строка пустая
	[[nodiscard]] const char* GetStringData() const
	{
		if (m_data.Empty()) return "";
		return &m_data[0];
	}

	// возвращает подстроку с заданной позиции длиной не больше length символов
	[[nodiscard]] MyString SubString(const size_t start, const size_t length = SIZE_MAX) const
	{
		const size_t currentLen = GetLength();
		if (start > currentLen)
		{
			throw std::out_of_range("Start index is out of range");
		}

		if (start == currentLen || length == 0)
		{
			return MyString();
		}

		const size_t available = currentLen - start;
		const size_t actualLength = length > available ? available : length;

		return MyString(&m_data[start], actualLength);
	}

	// очистка строки (строка становится снова нулевой длины)
	void Clear()
	{
		m_data.Clear();
		m_data.PushBack('\0');
	}

	// Возвращает вместимость строки
	[[nodiscard]] constexpr size_t GetCapacity() const
	{
		return m_data.Capacity() > 0 ? m_data.Capacity() - 1 : 0;
	}

	// оператор вывода строки в поток
	friend std::ostream& operator<<(std::ostream& ostream, const MyString& string)
	{
		ostream << string.GetStringData();
		return ostream;
	}

	// оператор ввода строки из потока
	// считывает символы до первого пробельного символа
	friend std::istream& operator>>(std::istream& istream, MyString& string)
	{
		string.Clear();

		char ch;
		if (!(istream >> ch))
		{
			return istream;
		}

		do
		{
			string += MyString(&ch, 1);
		} while (istream.get(ch) && !std::isspace(static_cast<unsigned char>(ch)));

		if (std::isspace(static_cast<unsigned char>(ch)))
		{
			istream.putback(ch);
		}

		return istream;
	}

	// функция для считывания строки целиком до символа-разделителя
	friend std::istream& GetLine(std::istream& istream, MyString& string, const char delim = '\n')
	{
		string.Clear();
		char ch;

		while (istream.get(ch) && ch != delim)
		{
			string += MyString(&ch, 1);
		}

		return istream;
	}

	// возвращает итератор,
	// указывающий на первый символ строки
	[[nodiscard]] constexpr Iterator begin() noexcept
	{
		return m_data.begin();
	}

	// возвращает итератор,
	// указывающий на позицию после последнего символа строки
	[[nodiscard]] constexpr Iterator end() noexcept
	{
		return m_data.begin() + GetLength();
	}

	// возвращает константный итератор,
	// указывающий на первый символ строки
	[[nodiscard]] constexpr ConstIterator begin() const noexcept
	{
		return m_data.cbegin();
	}

	// возвращает константный итератор,
	// указывающий на позицию после последнего символа строки
	[[nodiscard]] constexpr ConstIterator end() const noexcept
	{
		return m_data.cbegin() + GetLength();
	}

	// возвращает константный итератор,
	// указывающий на первый символ строки
	[[nodiscard]] constexpr ConstIterator cbegin() const noexcept
	{
		return m_data.cbegin();
	}

	// возвращает константный итератор,
	// указывающий на позицию после последнего символа строки
	[[nodiscard]] constexpr ConstIterator cend() const noexcept
	{
		return m_data.cbegin() + GetLength();
	}

	// возвращает обратный итератор,
	// указывающий на последний символ строки
	[[nodiscard]] constexpr ReverseIterator rbegin() noexcept
	{
		return ReverseIterator(end());
	}

	// возвращает обратный итератор,
	// указывающий на позицию перед первым символом строки
	[[nodiscard]] constexpr ReverseIterator rend() noexcept
	{
		return ReverseIterator(begin());
	}

	// возвращает константный обратный итератор,
	// указывающий на последний символ строки
	[[nodiscard]] constexpr ConstReverseIterator rbegin() const noexcept
	{
		return ConstReverseIterator(end());
	}

	// возвращает константный обратный итератор,
	// указывающий на позицию перед первым символом строки
	[[nodiscard]] constexpr ConstReverseIterator rend() const noexcept
	{
		return ConstReverseIterator(begin());
	}

	// возвращает константный обратный итератор,
	// указывающий на последний символ строки
	[[nodiscard]] constexpr ConstReverseIterator crbegin() const noexcept
	{
		return ConstReverseIterator(cend());
	}

	// возвращает константный обратный итератор,
	// указывающий на позицию перед первым символом строки
	[[nodiscard]] constexpr ConstReverseIterator crend() const noexcept
	{
		return ConstReverseIterator(cbegin());
	}

private:
	MyArray<char> m_data;

	// вспомогательный метод для добавления данных к текущей строке
	// используется для реализации операторов конкатенации
	void Append(const char* pString, const size_t length)
	{
		if (length == 0 || pString == nullptr)
		{
			return;
		}

		const bool isSelfCopy = pString >= m_data.begin() && pString < m_data.end();
		const size_t offset = isSelfCopy ? pString - m_data.begin() : 0;

		if (const size_t newTotalSize = GetLength() + length + 1; newTotalSize > m_data.Capacity())
		{
			m_data.Reserve(std::max(newTotalSize, m_data.Capacity() * 2));
		}

		if (isSelfCopy)
		{
			pString = m_data.begin() + offset;
		}

		if (!m_data.Empty() && m_data.Back() == '\0')
		{
			m_data.PopBack();
		}

		for (size_t i = 0; i < length; ++i)
		{
			m_data.PushBack(pString[i]);
		}

		m_data.PushBack('\0');
	}
};