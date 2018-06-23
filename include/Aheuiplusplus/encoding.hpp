#ifndef AHEUIPLUSPLUS_HEADER_ENCODING_HPP
#define AHEUIPLUSPLUS_HEADER_ENCODING_HPP

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace app
{
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

		class utf16be final
		{
		public:
			utf16be() = delete;
			utf16be(const utf16be& encoding) = delete;
			utf16be(utf16be&& encoding) noexcept = delete;
			~utf16be() = default;

		public:
			utf16be& operator=(const utf16be& encoding) = delete;
			utf16be& operator=(utf16be&& encoding) noexcept = delete;
			bool operator==(const utf16be& encoding) const = delete;
			bool operator!=(const utf16be& encoding) const = delete;

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
			static constexpr std::uint32_t codepage = 1201;
			static constexpr bool is_little_endian = false;
			static constexpr int max_encoded_length = 4;
			static constexpr std::uint8_t byte_order_mark[] = { 0xFE, 0xFF };
		};
	}	
}

#endif