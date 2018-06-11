#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <string>

namespace app
{
	using raw_code = std::u32string;

	bool is_complete_hangul(char32_t character) noexcept;
	char32_t get_chosung(char32_t hangul);
	char32_t get_jungsung(char32_t hangul);
	char32_t get_jongsung(char32_t hangul);
	char32_t get_jungsung_original(char32_t jungsung);
	char32_t is_added_additional_data(char32_t jungsung);
}

#endif