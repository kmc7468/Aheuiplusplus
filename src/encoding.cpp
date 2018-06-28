#include <Aheuiplusplus/encoding.hpp>

#include <algorithm>

namespace app
{
	bool is_little_endian()
	{
		static bool is_little_endian;
		static bool is_first = true;

		if (is_first)
		{
			is_first = false;
			std::uint32_t test = 1;

			is_little_endian = *reinterpret_cast<std::uint8_t*>(&test) == 1;

			if (!is_little_endian && *(reinterpret_cast<std::uint8_t*>(&test) + 3) != 1)
			{
				is_first = false;
				throw std::runtime_error("빅 엔디안 또는 리틀 엔디안을 사용하는 플랫폼만 지원합니다.");
			}
		}

		return is_little_endian;
	}

	namespace encoding
	{
		std::string utf8::encode(char32_t character)
		{
			std::string result;

			if (character < 0x80)
			{
				result.push_back(static_cast<char>(character));
			}
			else if (character < 0x0800)
			{
				result.push_back(static_cast<char>(0xC0 | (character >> 6)));
				result.push_back(static_cast<char>(0x80 | (character & 0x3F)));
			}
			else if (character < 0x10000)
			{
				result.push_back(static_cast<char>(0xE0 | (character >> 12)));
				result.push_back(static_cast<char>(0x80 | ((character >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (character & 0x3F)));
			}
			else if (character < 0x110000)
			{
				result.push_back(static_cast<char>(0xF0 | (character >> 18)));
				result.push_back(static_cast<char>(0x80 | ((character >> 12) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | ((character >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (character & 0x3F)));
			}
			else
				throw std::invalid_argument("인수 character는 올바른 유니코드 문자가 아닙니다.");

			return result;
		}		
		char32_t utf8::decode(const std::string& string)
		{
			if (static_cast<unsigned char>(string[0]) < 0x80)
			{
				return string[0];
			}
			else if ((static_cast<unsigned char>(string[0]) & 0xF0) == 0xF0)
			{
				return ((static_cast<std::int32_t>(string[0]) & 0x07) << 18) + ((static_cast<std::int32_t>(string[1]) & 0x3F) << 12)
					+ ((static_cast<std::int32_t>(string[2]) & 0x3F) << 6) + (static_cast<std::int32_t>(string[3]) & 0x3F);
			}
			else if ((static_cast<unsigned char>(string[0]) & 0xE0) == 0xE0)
			{
				return ((static_cast<std::int32_t>(string[0]) & 0x0F) << 12) + ((static_cast<std::int32_t>(string[1]) & 0x3F) << 6)
					+ (static_cast<std::int32_t>(string[2]) & 0x3F);
			}
			else if ((static_cast<unsigned char>(string[0]) & 0xC0) == 0xC0)
			{
				return ((static_cast<std::int32_t>(string[0]) & 0x1F) << 6) + (static_cast<std::int32_t>(string[1]) & 0x3F);
			}
			else
				throw std::invalid_argument("인수 string은 UTF-8로 올바르게 인코딩 되지 않았습니다.");
		}
		char32_t utf8::decode(char first, char second, char third, char fourth)
		{
			if (static_cast<unsigned char>(first) < 0x80)
			{
				return first;
			}
			else if ((static_cast<unsigned char>(first) & 0xF0) == 0xF0)
			{
				return ((static_cast<std::int32_t>(first) & 0x07) << 18) + ((static_cast<std::int32_t>(second) & 0x3F) << 12)
					+ ((static_cast<std::int32_t>(third) & 0x3F) << 6) + (static_cast<std::int32_t>(fourth) & 0x3F);
			}
			else if ((static_cast<unsigned char>(first) & 0xE0) == 0xE0)
			{
				return ((static_cast<std::int32_t>(first) & 0x0F) << 12) + ((static_cast<std::int32_t>(second) & 0x3F) << 6)
					+ (static_cast<std::int32_t>(third) & 0x3F);
			}
			else if ((static_cast<unsigned char>(first) & 0xC0) == 0xC0)
			{
				return ((static_cast<std::int32_t>(first) & 0x1F) << 6) + (static_cast<std::int32_t>(second) & 0x3F);
			}
			else
				throw std::invalid_argument("인수 string은 UTF-8로 올바르게 인코딩 되지 않았습니다.");
		}
		int utf8::encoded_length(char32_t character)
		{
			if (character < 0x80)
			{
				return 1;
			}
			else if (character < 0x800)
			{
				return 2;
			}
			else if (character < 0x10000)
			{
				return 3;
			}
			else if (character < 0x110000)
			{
				return 4;
			}
			else
				throw std::invalid_argument("인수 character는 올바른 유니코드 문자가 아닙니다.");
		}
		int utf8::encoded_length(char first)
		{
			if (static_cast<unsigned char>(first) < 0x80)
			{
				return 1;
			}
			else if ((static_cast<unsigned char>(first) & 0xF0) == 0xF0)
			{
				return 4;
			}
			else if ((static_cast<unsigned char>(first) & 0xE0) == 0xE0)
			{
				return 3;
			}
			else if ((static_cast<unsigned char>(first) & 0xC0) == 0xC0)
			{
				return 2;
			}
			else
				throw std::invalid_argument("인수 first는 UTF-8로 제대로 인코딩 되지 않았습니다.");
		}

		const std::string utf8::name = "UTF-8";
	}
	
	namespace encoding
	{
		std::u16string utf16::encode(char32_t character)
		{
			std::u16string result;

			if (character <= 0xFFFF)
			{
				result.push_back(static_cast<char16_t>(character));
				
				if (!app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[0]),
						reinterpret_cast<std::uint8_t*>(&result[0]) + 2);
				}
			}
			else
			{
				result.resize(2);

				char32_t temp = character - 0x10000;

				char16_t high_surrogate = static_cast<char16_t>((temp / 0x400) + 0xD800);
				char16_t low_surrogate = static_cast<char16_t>((temp % 0x400) + 0xDC00);

				result[0] = high_surrogate;
				result[1] = low_surrogate;

				if (!app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[0]),
						reinterpret_cast<std::uint8_t*>(&result[0]) + 2);
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[1]),
							reinterpret_cast<std::uint8_t*>(&result[1]) + 2);
				}
			}

			return result;
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		std::wstring utf16::wencode(char32_t character)
		{
			std::wstring result;
			std::u16string temp_result = encode(character);

			result.insert(result.end(), temp_result.begin(), temp_result.end());

			return result;
		}
#endif
		char32_t utf16::decode(const std::u16string& string)
		{
			char16_t first;
			char16_t second;

			first = string[0];

			if (!app::is_little_endian())
			{
				std::reverse(reinterpret_cast<std::uint8_t*>(&first),
					reinterpret_cast<std::uint8_t*>(&first) + 2);
			}

			if (encoded_length(first) == 2)
			{
				return first;
			}
			else
			{
				second = string[1];

				if (!app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&second),
						reinterpret_cast<std::uint8_t*>(&second) + 2);
				}

				return (first << 16) + second;
			}		
		}
		char32_t utf16::decode(char16_t first, char16_t second)
		{
			if (!app::is_little_endian())
			{
				std::reverse(reinterpret_cast<std::uint8_t*>(&first),
					reinterpret_cast<std::uint8_t*>(&first) + 2);
			}

			if (encoded_length(first) == 2)
			{
				return first;
			}
			else
			{
				if (!app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&second),
						reinterpret_cast<std::uint8_t*>(&second) + 2);
				}

				return (first << 16) + second;
			}
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		char32_t utf16::wdecode(const std::wstring& string)
		{
			if (string.length())
			{
				return decode(static_cast<char16_t>(string[0]));
			}
			else
			{
				return decode(static_cast<char16_t>(string[0]), static_cast<char16_t>(string[1]));
			}
		}
		char32_t utf16::wdecode(wchar_t first, wchar_t second)
		{
			return decode(first, second);
		}
#endif
		int utf16::encoded_length(char32_t character)
		{
			if (character <= 0xFF)
			{
				return 2;
			}
			else
			{
				return 4;
			}
		}
		int utf16::encoded_length(char16_t first)
		{
			if (first >= 0xD800 && first <= 0xDBFF)
			{
				return 4;
			}
			else
			{
				return 2;
			}
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		int utf16::wencoded_length(wchar_t character)
		{
			return encoded_length(static_cast<char16_t>(character));
		}
#endif

		const std::string utf16::name = "UTF-16";
	}

	namespace encoding
	{
		std::u16string utf16be::encode(char32_t character)
		{
			std::u16string result;

			if (character <= 0xFFFF)
			{
				result.push_back(static_cast<char16_t>(character));

				if (app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[0]),
						reinterpret_cast<std::uint8_t*>(&result[0]) + 2);
				}
			}
			else
			{
				result.resize(2);

				char32_t temp = character - 0x10000;

				char16_t high_surrogate = static_cast<char16_t>((temp / 0x400) + 0xD800);
				char16_t low_surrogate = static_cast<char16_t>((temp % 0x400) + 0xDC00);

				result[0] = high_surrogate;
				result[1] = low_surrogate;

				if (app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[0]),
						reinterpret_cast<std::uint8_t*>(&result[0]) + 2);
					std::reverse(reinterpret_cast<std::uint8_t*>(&result[1]),
						reinterpret_cast<std::uint8_t*>(&result[1]) + 2);
				}
			}

			return result;
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		std::wstring utf16be::wencode(char32_t character)
		{
			std::wstring result;
			std::u16string temp_result = encode(character);

			result.insert(result.end(), temp_result.begin(), temp_result.end());

			return result;
		}
#endif
		char32_t utf16be::decode(const std::u16string& string)
		{
			char16_t first;
			char16_t second;

			first = string[0];

			if (app::is_little_endian())
			{
				std::reverse(reinterpret_cast<std::uint8_t*>(&first),
					reinterpret_cast<std::uint8_t*>(&first) + 2);
			}

			if (encoded_length(first) == 2)
			{
				return first;
			}
			else
			{
				second = string[1];

				if (app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&second),
						reinterpret_cast<std::uint8_t*>(&second) + 2);
				}

				return (first << 16) + second;
			}
		}
		char32_t utf16be::decode(char16_t first, char16_t second)
		{
			if (!app::is_little_endian())
			{
				std::reverse(reinterpret_cast<std::uint8_t*>(&first),
					reinterpret_cast<std::uint8_t*>(&first) + 2);
			}

			if (encoded_length(first) == 2)
			{
				return first;
			}
			else
			{
				if (app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&second),
						reinterpret_cast<std::uint8_t*>(&second) + 2);
				}

				return (first << 16) + second;
			}
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		char32_t utf16be::wdecode(const std::wstring& string)
		{
			if (string.length())
			{
				return decode(static_cast<char16_t>(string[0]));
			}
			else
			{
				return decode(static_cast<char16_t>(string[0]), static_cast<char16_t>(string[1]));
			}
		}
		char32_t utf16be::wdecode(wchar_t first, wchar_t second)
		{
			return decode(first, second);
		}
#endif
		int utf16be::encoded_length(char32_t character)
		{
			if (character <= 0xFF)
			{
				return 2;
			}
			else
			{
				return 4;
			}
		}
		int utf16be::encoded_length(char16_t first)
		{
			if (first >= 0xD800 && first <= 0xDBFF)
			{
				return 4;
			}
			else
			{
				return 2;
			}
		}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		int utf16be::wencoded_length(wchar_t character)
		{
			return encoded_length(static_cast<char16_t>(character));
		}
#endif

		const std::string utf16be::name = "UTF-16BE";
	}
}