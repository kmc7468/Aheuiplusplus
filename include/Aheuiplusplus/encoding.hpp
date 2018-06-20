#ifndef AHEUIPLUSPLUS_HEADER_ENCODING_HPP
#define AHEUIPLUSPLUS_HEADER_ENCODING_HPP

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace app
{
	bool is_little_endian();
	template<typename Ty_>
	std::uint8_t get_byte(const Ty_& data, int index)
	{
		if (index >= sizeof(Ty_))
			throw std::invalid_argument("인수 index는 템플릿 인수 Ty_의 크기 미만이여야 합니다.");
		
		return *(reinterpret_cast<const std::uint8_t*>(&data) + index);
	}
	template<typename Ty_>
	std::uint8_t get_byte_in_little_endian(const Ty_& data, int index)
	{
		if (index >= sizeof(Ty_))
			throw std::invalid_argument("인수 index는 템플릿 인수 Ty_의 크기 미만이여야 합니다.");
		
		if (is_little_endian())
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + index);
		}
		else
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + ((sizeof(Ty_) - 1) - index));
		}
	}
	template<typename Ty_>
	std::uint8_t get_byte_in_big_endian(const Ty_& data, int index)
	{
		if (index >= sizeof(Ty_))
			throw std::invalid_argument("인수 index는 템플릿 인수 Ty_의 크기 미만이여야 합니다.");

		if (is_little_endian())
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + ((sizeof(Ty_) - 1) - index));
		}
		else
		{
			return *(reinterpret_cast<const std::uint8_t*>(&data) + index);
		}
	}

	namespace encoding
	{
		class utf8 final
		{
		public:
			utf8() = delete;
			utf8(const utf8& encoding) = delete;
			utf8(utf8&& encoding) noexcept = delete;
			~utf8() = default;

		public:
			utf8& operator=(const utf8& encoding) = delete;
			utf8& operator=(utf8&& encoding) noexcept = delete;
			bool operator==(const utf8& encoding) const = delete;
			bool operator!=(const utf8& encoding) const = delete;

		public:
			static std::string encode(char32_t character);
			static char32_t decode(const std::string& string);
			static char32_t decode(char first, char second = 0, char third = 0, char fourth = 0);
			static int encoded_length(char32_t character);
			static int encoded_length(char first);

		public:
			static const std::string name;
			static constexpr std::uint32_t codepage = 65001;
			static constexpr bool is_little_endian = false;
			static constexpr int max_encoded_length = 4;
			static constexpr std::uint8_t byte_order_mark[] = { 0xEF, 0xBB, 0xBF };
		};

		class utf16 final
		{
		public:
			utf16() = delete;
			utf16(const utf16& encoding) = delete;
			utf16(utf16&& encoding) noexcept = delete;
			~utf16() = default;

		public:
			utf16& operator=(const utf16& encoding) = delete;
			utf16& operator=(utf16&& encoding) noexcept = delete;
			bool operator==(const utf16& encoding) const = delete;
			bool operator!=(const utf16& encoding) const = delete;

		public:
			static std::u16string encode(char32_t character);
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			static std::wstring wencode(char32_t character);
#endif
			static char32_t decode(const std::u16string& string);
			static char32_t decode(char16_t first, char16_t second = 0);
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			static char32_t wdecode(const std::wstring& string);
			static char32_t wdecode(wchar_t first, wchar_t second = 0);
#endif
			static int encoded_length(char32_t character);
			static int encoded_length(char16_t character);
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			static int wencoded_length(wchar_t character);
#endif

		public:
			static const std::string name;
			static constexpr std::uint32_t codepage = 1200;
			static constexpr bool is_little_endian = true;
			static constexpr int max_encoded_length = 4;
			static constexpr std::uint8_t byte_order_mark[] = { 0xFF, 0xFE };
		};
	}	
}

#endif