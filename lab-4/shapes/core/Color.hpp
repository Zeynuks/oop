#pragma once

#include <cstdint>
#include <format>
#include <string_view>

struct Color
{
	constexpr Color() = default;

	constexpr Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}

	explicit constexpr Color(const uint32_t hex)
		: r(static_cast<uint8_t>(hex >> 24 & 0xFF))
		, g(static_cast<uint8_t>(hex >> 16 & 0xFF))
		, b(static_cast<uint8_t>(hex >> 8 & 0xFF))
		, a(static_cast<uint8_t>(hex & 0xFF))
	{
	}

	auto operator<=>(const Color&) const = default;

	constexpr uint32_t ToInt() const
	{
		return static_cast<uint32_t>(r) << 24 | static_cast<uint32_t>(g) << 16 | static_cast<uint32_t>(b) << 8 | static_cast<uint32_t>(a);
	}

	explicit constexpr operator uint32_t() const
	{
		return ToInt();
	}

	constexpr Color& operator=(const uint32_t hex)
	{
		r = static_cast<uint8_t>(hex >> 24 & 0xFF);
		g = static_cast<uint8_t>(hex >> 16 & 0xFF);
		b = static_cast<uint8_t>(hex >> 8 & 0xFF);
		a = static_cast<uint8_t>(hex & 0xFF);

		return *this;
	}

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;
};

template <>
struct std::formatter<Color> : std::formatter<std::string_view>
{
	static auto format(const Color& c, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{:08x}", c.ToInt());
	}
};

namespace DefaultColors
{
static constexpr Color Red{ 255, 0, 0 };
static constexpr Color Green{ 0, 255, 0 };
static constexpr Color Blue{ 0, 0, 255 };
static constexpr Color Yellow{ 255, 255, 0 };
static constexpr Color Magenta{ 255, 0, 255 };
static constexpr Color Cyan{ 0, 255, 255 };
static constexpr Color White{ 255, 255, 255 };
static constexpr Color Black{ 0, 0, 0 };
static constexpr Color Gray{ 128, 128, 128 };
static constexpr Color Transparent{ 0, 0, 0, 0 };
} // namespace DefaultColors