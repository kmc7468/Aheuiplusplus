#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <cstdio>
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
	
	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung);
	char32_t get_complete_hangul(char32_t chosung, char32_t jungsung, char32_t jongsung);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	std::wstring read_wchar(std::FILE* input_stream);
	void write_wchar(std::FILE* output_stream, const std::wstring& character);
#endif
	std::string read_u8char(std::FILE* input_stream);
	void write_u8char(std::FILE* input_stream, const std::string& character);

	char32_t read_char(std::FILE* input_stream);
	void write_char(std::FILE* input_stream, char32_t character);
	void unread_char(std::FILE* input_stream, char32_t character);

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