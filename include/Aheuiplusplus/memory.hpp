#ifndef AHEUIPLUSPLUS_HEADER_MEMORY_HPP
#define AHEUIPLUSPLUS_HEADER_MEMORY_HPP

#include <cstdint>

namespace app
{
	bool is_little_endian();

	template<typename Ty_, int Index_>
	std::uint8_t get_byte(const Ty_& data)
	{
		static_assert(Index_ < sizeof(Ty_), "Template Argument Index_ must be less than the size of template argument Ty_.");

		return *(reinterpret_cast<const std::uint8_t*>(&data) + index);
	}
	template<typename Ty_, int Index_>
	std::uint8_t get_byte_by_little_endian(const Ty_& data)
	{
		static_assert(Index_ < sizeof(Ty_), "Template Argument Index_ must be less than the size of template argument Ty_.");

		if (is_little_endian())
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + Index_);
		}
		else
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + ((sizeof(Ty_) - 1) - Index_));
		}
	}
	template<typename Ty_, int Index_>
	std::uint8_t get_byte_by_big_endian(const Ty_& data)
	{
		static_assert(Index_ < sizeof(Ty_), "Template Argument Index_ must be less than the size of template argument Ty_.");

		if (is_little_endian())
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + ((sizeof(Ty_) - 1) - Index_));
		}
		else
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + Index_);
		}
	}
}

#endif