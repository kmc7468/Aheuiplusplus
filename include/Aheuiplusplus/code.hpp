#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <string>
#include <vector>

namespace app
{
	using raw_code = std::u32string;

	bool is_complete_hangul(char32_t character) noexcept;
	char32_t get_chosung(char32_t hangul);
	char32_t get_jungsung(char32_t hangul);
	char32_t get_jongsung(char32_t hangul);
	char32_t get_jungsung_original(char32_t jungsung);
	char32_t is_added_additional_data(char32_t jungsung);

	std::wstring char32_to_wchar(char32_t character);
	char32_t wchar_to_char32(wchar_t high_surrogate, wchar_t low_surrogate);

	class code final
	{
	public:
		code() = default;
		code(const raw_code& code);
		code(const code& code);
		code(code&& code) noexcept;
		~code() = default;

	public:
		code& operator=(const raw_code& code);
		code& operator=(const code& code);
		code& operator=(code&& code) noexcept;
		bool operator==(const code& code) const = delete;
		bool operator!=(const code& code) const = delete;

	public:
		char32_t command(std::size_t x, std::size_t y) const;
		char32_t& command(std::size_t x, std::size_t y);
		const raw_code& line(std::size_t y) const;
		raw_code& line(std::size_t y);

	private:
		void parse_raw_code_(const raw_code& code);

	public:
		const std::vector<raw_code>& codes() const noexcept;
		std::vector<raw_code>& codes() noexcept;

	private:
		std::vector<raw_code> codes_;
	};
}

#endif