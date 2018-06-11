#include <Aheuiplusplus/code.hpp>

#include <stdexcept>
#include <utility>

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
			return U'ㅞ';

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