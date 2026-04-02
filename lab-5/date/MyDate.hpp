#pragma once

#include <chrono>
#include <expected>
#include <format>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <tuple>

enum class Month
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

enum class WeekDay
{
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
};

//TODO unsigned to uint64_t
class MyDate
{
private:
	unsigned daysEpoch = 719'468;
	unsigned maxEpochDays = 2'932'896;
public:
	constexpr MyDate(const unsigned day, const Month month, const unsigned year)
	{
		const auto unsignedMonth = static_cast<unsigned>(month);
		IsValid(day, unsignedMonth, year);
		m_epochDays = FromDate(day, unsignedMonth, year);
	}

	constexpr MyDate(const unsigned day, const unsigned month, const unsigned year)
	{
		IsValid(day, month, year);
		m_epochDays = FromDate(day, month, year);
	}

	//подумать как измбежать неявного преобразования типов
	constexpr explicit MyDate(const unsigned epochDays)
	{
		IsEpochInRange(epochDays);
		m_epochDays = epochDays;
	}

	constexpr MyDate() = default;

	[[nodiscard]] constexpr unsigned GetDay() const
	{
		auto [day, month, year] = ToDate(m_epochDays);
		return day;
	}

	[[nodiscard]] constexpr Month GetMonth() const
	{
		auto [day, month, year] = ToDate(m_epochDays);
		return static_cast<Month>(month);
	}

	[[nodiscard]] constexpr unsigned GetYear() const
	{
		auto [day, month, year] = ToDate(m_epochDays);
		return year;
	}

	[[nodiscard]] constexpr WeekDay GetWeekDay() const
	{
		const unsigned weekday = (m_epochDays + 4) % 7;
		return static_cast<WeekDay>(weekday);
	}

	[[nodiscard]] constexpr std::string ToString() const
	{
		auto [day, month, year] = ToDate(m_epochDays);
		return std::format("{:02}.{:02}.{}", day, month, year);
	}

	[[nodiscard]] static constexpr MyDate Min()
	{
		return MyDate();
	}

	[[nodiscard]] static constexpr MyDate Max()
	{
		return MyDate(maxEpochDays);
	}

	[[nodiscard]] static MyDate Now()
	{
		const auto now = std::chrono::system_clock::now();
		const auto epochDays = std::chrono::duration_cast<std::chrono::days>(now.time_since_epoch());
		return MyDate(static_cast<unsigned>(epochDays.count()));
	}

	static constexpr void IsValid(const unsigned day, const unsigned month, const unsigned year)
	{
		constexpr unsigned maxYear = 9999;
		constexpr unsigned minYear = 1970;
		constexpr unsigned maxMonth = 12;
		constexpr unsigned minMonth = 1;

		if (day == 0 || day > LastDayOfMonth(month, year))
		{
			throw std::out_of_range("Day is out of range of given month");
		}

		if (month < minMonth || month > maxMonth)
		{
			throw std::out_of_range("Month must be in range from 1 to 12");
		}

		if (year < minYear || year > maxYear)
		{
			throw std::out_of_range("Year must be in range from 1970 to 9999");
		}
	}

	constexpr MyDate& operator++()
	{
		const auto next = m_epochDays + 1;
		IsEpochInRange(next);
		m_epochDays = next;
		return *this;
	}

	[[nodiscard]] constexpr MyDate operator++(int) noexcept
	{
		const auto temp = *this;
		++*this;
		return temp;
	}

	constexpr MyDate& operator--()
	{
		const auto prev = m_epochDays - 1;
		IsEpochInRange(prev);
		m_epochDays = prev;
		return *this;
	}

	[[nodiscard]] constexpr MyDate operator--(int) noexcept
	{
		const auto temp = *this;
		--*this;
		return temp;
	}

	[[nodiscard]] constexpr MyDate operator+(const int days) const
	{
		const auto newEpochDays = m_epochDays + days;
		IsEpochInRange(newEpochDays);
		return MyDate(newEpochDays);
	}

	[[nodiscard]] constexpr MyDate operator-(const int days) const
	{
		const auto newEpochDays = m_epochDays - days;
		IsEpochInRange(newEpochDays);
		return MyDate(newEpochDays);
	}

	[[nodiscard]] constexpr int operator-(const MyDate& other) const noexcept
	{
		return static_cast<int>(m_epochDays) - static_cast<int>(other.m_epochDays);
	}

	constexpr MyDate& operator+=(const int days)
	{
		*this = *this + days;
		return *this;
	}

	constexpr MyDate& operator-=(const int days)
	{
		*this = *this - days;
		return *this;
	}

	//каким образом раелизуется комплятором и почему работает default
	[[nodiscard]] constexpr auto operator<=>(const MyDate& other) const noexcept = default;

	friend std::ostream& operator<<(std::ostream& out, const MyDate& date)
	{
		return out << date.ToString();
	}

	friend std::istream& operator>>(std::istream& input, MyDate& date)
	{
		unsigned day;
		unsigned month;
		unsigned year;
		char dot1;
		char dot2;

		if (!(input >> day >> dot1 >> month >> dot2 >> year))
		{
			return input;
		}

		if (dot1 != '.' || dot2 != '.')
		{
			input.setstate(std::ios::failbit);
			return input;
		}

		try
		{
			date = MyDate(day, month, year);
			input.setstate(std::ios::goodbit);
		}
		catch (const std::exception&)
		{
			input.setstate(std::ios::failbit);
		}

		return input;
	}

private:
	unsigned m_epochDays = 0;

	// Реализовано благодаря: Howard Hinnant
	// https://howardhinnant.github.io/date_algorithms.html#civil_from_days
	static constexpr unsigned FromDate(const unsigned day, const unsigned month, unsigned year) noexcept
	{
		year -= month <= 2;
		const unsigned era = year / 400;
		const unsigned yoe = year - era * 400;
		const unsigned doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1;
		const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
		return era * 146097 + doe - 719468;
	}

	// Реализовано благодаря: Howard Hinnant
	// https://howardhinnant.github.io/date_algorithms.html#civil_from_days
	static constexpr std::tuple<unsigned, unsigned, unsigned> ToDate(const unsigned epochDays) noexcept
	{
		const unsigned z = epochDays + daysEpoch;
		const unsigned era = z / 146097;
		const unsigned doe = z - era * 146097;
		const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
		const unsigned y = yoe + era * 400;
		const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
		const unsigned mp = (5 * doy + 2) / 153;
		const unsigned d = doy - (153 * mp + 2) / 5 + 1;
		const unsigned m = mp < 10 ? mp + 3 : mp - 9;

		return std::tuple(d, m, y + (m <= 2));
	}

	static constexpr void IsEpochInRange(const unsigned epochDays)
	{
		if (epochDays > maxEpochDays)
		{
			throw std::out_of_range("MyDate is out of range");
		}
	}

	static constexpr bool IsLeap(const unsigned y) noexcept
	{
		return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
	}

	static constexpr unsigned LastDayOfMonthCommonYear(const unsigned m) noexcept
	{
		constexpr unsigned char a[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return a[m - 1];
	}

	// Реализовано благодаря: Howard Hinnant
	// https://howardhinnant.github.io/date_algorithms.html#civil_from_days
	static constexpr unsigned LastDayOfMonth(const unsigned m, const unsigned y) noexcept
	{
		if (m == 2 && IsLeap(y))
		{
			return 29u;
		}

		return LastDayOfMonthCommonYear(m);
	}
};

template <>
struct std::formatter<MyDate> : std::formatter<std::string>
{
	auto format(const MyDate& date, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(date.ToString(), ctx);
	}
};