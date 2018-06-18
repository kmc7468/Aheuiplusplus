#include <Aheuiplusplus/code.hpp>

#include <cstdint>
#include <stdexcept>
#include <utility>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	bool is_complete_hangul(char32_t character) noexcept
	{
		return character >= 0xAC00 && character <= 0xD7A3;
	}
	char32_t get_chosung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("인수 hangul은 완성된 현대 한글이여야 합니다.");

		static constexpr char32_t chosungs[] = {
			U'ㄱ', U'ㄲ', U'ㄴ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ',
			U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
		};

		return chosungs[((((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) - ((((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) % 21)) / 21];
	}
	char32_t get_jungsung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("인수 hangul은 완성된 현대 한글이여야 합니다.");

		static constexpr char32_t jungsungs[] = {
			U'ㅏ', U'ㅐ', U'ㅑ', U'ㅒ', U'ㅓ', U'ㅔ', U'ㅕ', U'ㅖ', U'ㅗ', U'ㅘ', U'ㅙ', U'ㅚ', U'ㅛ',
			U'ㅜ', U'ㅝ', U'ㅞ', U'ㅟ', U'ㅠ', U'ㅡ', U'ㅢ', U'ㅣ'
		};

		return jungsungs[(((hangul - 0xAC00) - ((hangul - 0xAC00) % 28)) / 28) % 21];
	}
	char32_t get_jongsung(char32_t hangul)
	{
		if (!is_complete_hangul(hangul))
			throw std::invalid_argument("인수 hangul은 완성된 현대 한글이여야 합니다.");

		static constexpr char32_t jongsungs[] = {
			0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
			U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
		};

		return jongsungs[(hangul - 0xAC00) % 28];
	}
	char32_t get_jungsung_original(char32_t jungsung)
	{
		if (jungsung < 0x314F || jungsung > 0x3163)
			throw std::invalid_argument("인수 jungsung은 현대 한글 모음이여야 합니다.");

		switch (jungsung)
		{
		case U'ㅏ':
		case U'ㅐ':
			return U'ㅏ';

		case U'ㅓ':
		case U'ㅔ':
			return U'ㅓ';

		case U'ㅗ':
		case U'ㅚ':
			return U'ㅗ';

		case U'ㅜ':
		case U'ㅟ':
			return U'ㅜ';

		case U'ㅑ':
		case U'ㅒ':
			return U'ㅑ';

		case U'ㅕ':
		case U'ㅖ':
			return U'ㅕ';

		case U'ㅛ':
		case U'ㅙ':
			return U'ㅛ';

		case U'ㅠ':
		case U'ㅞ':
			return U'ㅠ';

		case U'ㅣ':
		case U'ㅘ':
			return U'ㅣ';

		case U'ㅡ':
		case U'ㅝ':
			return U'ㅡ';

		default:
			return U'ㅢ';
			
		}
	}
	char32_t is_added_additional_data(char32_t jungsung)
	{
		return jungsung != get_jungsung_original(jungsung);
	}

	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung)
	{
		return get_complete_hangul(chosung, jungsung, 0);
	}
	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung, char32_t jongsung)
	{
		if (chosung < U'ㄱ' || chosung > U'ㅎ')
			throw std::invalid_argument("인수 chosung은 현대 한글 자음 중 초성에 올 수 있는 자음이여야 합니다.");
		if (jungsung < U'ㅏ' || jungsung > U'ㅣ')
			throw std::invalid_argument("인수 jungsung은 현대 한글 모음이여야 합니다.");
		if (jongsung != 0 && (jongsung < U'ㄱ' || jongsung > U'ㅎ'))
			throw std::invalid_argument("인수 jongsung은 현대 한글 자음 중 종성에 올 수 있는 자음이여야 합니다.");

		static constexpr char32_t chosungs[] = {
			U'ㄱ', U'ㄲ', U'ㄴ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ',
			U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
		};

		char32_t chosung_id = static_cast<char32_t>(-1);

		for (char32_t i = 0; i < sizeof(chosungs) / sizeof(char32_t); ++i)
		{
			if (chosungs[i] == chosung)
			{
				chosung_id = i;
				break;
			}
		}

		if (chosung_id == static_cast<char32_t>(-1))
			throw std::invalid_argument("인수 chosung은 현대 한글 자음 중 초성에 올 수 있는 자음이여야 합니다.");

		if (jongsung == 0)
		{
			return 588 * chosung_id + 28 * (jungsung - U'ㅏ') + 0xAC00;
		}
		else
		{
			static constexpr char32_t jongsungs[] = {
				0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
				U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
			};

			char32_t jongsung_id = static_cast<char32_t>(-1);

			for (char32_t i = 0; i < sizeof(jongsungs) / sizeof(char32_t); ++i)
			{
				if (jongsungs[i] == jongsung)
				{
					jongsung_id = i;
					break;
				}
			}

			if (jongsung_id == static_cast<char32_t>(-1))
				throw std::invalid_argument("인수 jongsung은 현대 한글 자음 중 종성에 올 수 있는 자음이여야 합니다.");

			return 588 * chosung_id + 28 * (jungsung - U'ㅏ') + jongsung_id + 0xAC00;
		}
	}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	std::wstring char32_to_wchar(char32_t character)
	{
		if (character <= 65'535)
		{
			return std::wstring(1, static_cast<wchar_t>(character));
		}

		std::wstring result;
		result.resize(2);

		char32_t temp = character - 0x10000;

		wchar_t high_surrogate = static_cast<wchar_t>((temp / 0x400) + 0xD800);
		wchar_t low_surrogate = static_cast<wchar_t>((temp % 0x400) + 0xDC00);

		result[0] = low_surrogate;
		result[1] = high_surrogate;

		return result;
	}
	char32_t wchar_to_char32(wchar_t high_surrogate, wchar_t low_surrogate)
	{
		if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
		{
			char32_t temp_high = (high_surrogate - 0xD800) * 0x400;
			char32_t temp_low = low_surrogate - 0xDC00;

			return temp_high + temp_low + 0x10000;
		}
		else
		{
			return high_surrogate;
		}
	}
#endif
	std::string char32_to_u8char(char32_t character)
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
		else
		{
			result.push_back(static_cast<char>(0xF0 | (character >> 18)));
			result.push_back(static_cast<char>(0x80 | ((character >> 12) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | ((character >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (character & 0x3F)));
		}

		return result;
	}
	char32_t u8char_to_char32(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth)
	{
		if (first < 0x80)
		{
			return first;
		}
		else if ((first & 0xF0) == 0xF0)
		{
			return ((static_cast<std::int32_t>(first) & 0x07) << 18) + ((static_cast<std::int32_t>(second) & 0x3F) << 12)
				+ ((static_cast<std::int32_t>(third) & 0x3F) << 6) + (static_cast<std::int32_t>(fourth) & 0x3F);
		}
		else if ((first & 0xE0) == 0xE0)
		{
			return ((static_cast<std::int32_t>(first) & 0x0F) << 12) + ((static_cast<std::int32_t>(second) & 0x3F) << 6)
				+ (static_cast<std::int32_t>(third) & 0x3F);
		}
		else if ((first & 0xC0) == 0xC0)
		{
			return ((static_cast<std::int32_t>(first) & 0x1F) << 6) + (static_cast<std::int32_t>(second) & 0x3F);
		}
	}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	int wchar_length(char32_t character)
	{
		if (character < 65'536)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	int wchar_length(wchar_t first)
	{
		if (first >= 0xD800 && first <= 0xDBFF)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
#endif
	int u8char_length(char32_t character)
	{
		if (character < 0x80)
		{
			return 1;
		}
		else if (character < 0x0800)
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
		{
			return -1;
		}
	}
	int u8char_length(unsigned char first)
	{
		if (first < 0x80)
		{
			return 1;
		}
		else if ((first & 0xF0) == 0xF0)
		{
			return 4;
		}
		else if ((first & 0xE0) == 0xE0)
		{
			return 3;
		}
		else if ((first & 0xC0) == 0xC0)
		{
			return 2;
		}
		else
		{
			return -1;
		}
	}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	std::wstring read_wchar(std::FILE* input_stream)
	{
		wchar_t high_surrogate = std::fgetwc(input_stream);

		if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
		{
			wchar_t low_surrogate = std::fgetwc(input_stream);

			return { high_surrogate, low_surrogate };
		}
		else
		{
			return { high_surrogate };
		}
	}
	void write_wchar(std::FILE* output_stream, const std::wstring& character)
	{
		for (std::size_t i = 0; i < character.length(); ++i)
		{
			std::fputwc(character[i], output_stream);
		}
	}
#endif
	std::string read_u8char(std::FILE* input_stream)
	{
		std::string result;

		unsigned char first = static_cast<unsigned char>(std::fgetc(input_stream));
		int length = app::u8char_length(first);

		if (length == 1)
		{
			result.push_back(first);
		}
		else if (length == 2)
		{
			unsigned char second = static_cast<unsigned char>(std::fgetc(input_stream));

			result.push_back(first);
			result.push_back(second);
		}
		else if (length == 3)
		{
			unsigned char second = static_cast<unsigned char>(std::fgetc(input_stream));
			unsigned char third = static_cast<unsigned char>(std::fgetc(input_stream));

			result.push_back(first);
			result.push_back(second);
			result.push_back(third);
		}
		else
		{
			unsigned char second = static_cast<unsigned char>(std::fgetc(input_stream));
			unsigned char third = static_cast<unsigned char>(std::fgetc(input_stream));
			unsigned char fourth = static_cast<unsigned char>(std::fgetc(input_stream));

			result.push_back(first);
			result.push_back(second);
			result.push_back(third);
			result.push_back(fourth);
		}

		return result;
	}
	void write_u8char(std::FILE* output_stream, const std::string& character)
	{
		for (std::size_t i = 0; i < character.length(); ++i)
		{
			std::fputc(character[i], output_stream);
		}
	}

	char32_t read_char(std::FILE* input_stream)
	{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(input_stream), _O_U16TEXT);

		wchar_t high_surrogate = std::fgetwc(input_stream);

		if (wchar_length(high_surrogate) == 1)
		{
			return high_surrogate;
		}
		else
		{
			wchar_t low_surroagte = std::fgetwc(input_stream);

			return wchar_to_char32(high_surrogate, low_surroagte);
		}
#else
		std::string input = read_u8char(input_stream);

		if (input.length() == 1)
		{
			return u8char_to_char32(input[0]);
		}
		else if (input.length() == 2)
		{
			return u8char_to_char32(input[0], input[1]);
		}
		else if (input.length() == 3)
		{
			return u8char_to_char32(input[0], input[1], input[2]);
		}
		else
		{
			return u8char_to_char32(input[0], input[1], input[2], input[3]);
		}
#endif
	}
	void write_char(std::FILE* output_stream, char32_t character)
	{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(output_stream), _O_U16TEXT);

		std::wstring utf16 = char32_to_wchar(character);

		write_wchar(output_stream, utf16);

		_setmode(_fileno(output_stream), _O_TEXT);
#else
		std::string utf8 = char32_to_u8char(character);

		write_u8char(output_stream, utf8);
#endif
	}
}

namespace app
{
	code::code(const raw_code& code)
	{
		parse_raw_code_(code);
	}
	code::code(const code& code)
		: codes_(code.codes_)
	{}
	code::code(code&& code) noexcept
		: codes_(std::move(code.codes_))
	{}

	code& code::operator=(const raw_code& code)
	{
		parse_raw_code_(code);
		return *this;
	}
	code& code::operator=(const code& code)
	{
		codes_ = code.codes_;
		return *this;
	}
	code& code::operator=(code&& code) noexcept
	{
		codes_ = std::move(code.codes_);
		return *this;
	}

	char32_t code::command(std::size_t x, std::size_t y) const
	{
		return codes_[y][x];
	}
	char32_t& code::command(std::size_t x, std::size_t y)
	{
		return codes_[y][x];
	}
	const raw_code& code::line(std::size_t y) const
	{
		return codes_[y];
	}
	raw_code& code::line(std::size_t y)
	{
		return codes_[y];
	}

	void code::parse_raw_code_(const raw_code& code)
	{
		codes_.clear();

		raw_code code_editable = code;
		code_editable.push_back(U'\n');
		raw_code splited;

		while (code_editable.find(U'\n') != raw_code::npos)
		{
			splited = code_editable.substr(0, code_editable.find(U'\n') + 1);
			codes_.push_back(splited);

			code_editable = code_editable.substr(code_editable.find(U'\n') + 1);
		}
	}

	const std::vector<raw_code>& code::codes() const noexcept
	{
		return codes_;
	}
	std::vector<raw_code>& code::codes() noexcept
	{
		return codes_;
	}
}