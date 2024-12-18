#pragma once

#include <type_traits>

namespace Reflection
{
	template<auto Bit>
	constexpr auto BIT = 1 << (Bit - 1);

	template<typename TEnum>
	struct BitMask final
	{
		static_assert(std::is_enum_v<TEnum>, "TEnum should be enum type");

	public:

		using MaskType = std::underlying_type_t<TEnum>;

	private:

		MaskType m_Mask = 0;

	public:

		constexpr BitMask() = default;
		constexpr BitMask(const BitMask&) = default;

		constexpr BitMask(TEnum bit) :
			m_Mask(static_cast<MaskType>(bit))
		{
		}

		constexpr BitMask(BitMask&& other) noexcept :
			m_Mask(other.m_Mask)
		{
			other.m_Mask = 0;
		}

		constexpr operator bool() const noexcept
		{
			return m_Mask != 0;
		}

		constexpr BitMask& operator=(const BitMask& other) noexcept
		{
			m_Mask = other.m_Mask;
			return *this;
		}

		constexpr MaskType Get() const noexcept
		{
			return m_Mask;
		}

		constexpr bool operator==(BitMask other) const noexcept
		{
			return m_Mask == other.m_Mask;
		}

		constexpr bool operator!=(BitMask other) const noexcept
		{
			return m_Mask != other.m_Mask;
		}

		constexpr BitMask operator|=(BitMask mask) noexcept
		{
			m_Mask &= mask.m_Mask;
			return *this;
		}

		constexpr BitMask operator&=(BitMask mask) noexcept
		{
			m_Mask &= mask.m_Mask;
			return *this;
		}

		constexpr BitMask operator&(BitMask mask) const noexcept
		{
			return static_cast<TEnum>(m_Mask & mask.m_Mask);
		}

		constexpr BitMask operator|(BitMask mask) const noexcept
		{
			return static_cast<TEnum>(m_Mask | mask.m_Mask);
		}

		constexpr BitMask operator^(MaskType mask) const noexcept
		{
			return static_cast<TEnum>(m_Mask ^ mask);
		}

		constexpr BitMask operator~() const noexcept
		{
			return static_cast<TEnum>(~m_Mask);
		}
	};
}
