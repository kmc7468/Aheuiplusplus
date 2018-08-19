#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <Aheuiplusplus/cursor.hpp>

#include <string>
#include <vector>

#include <u5e/basic_grapheme.hpp>

namespace app
{
	using grapheme = u5e::basic_grapheme<std::u32string>;

	class code final
	{
	public:
		code() = default;
		code(const std::u32string_view& codes);
		code(const code& code);
		code(code&& code) noexcept;
		~code() = default;

	public:
		code& operator=(const std::u32string_view& codes);
		code& operator=(const code& code);
		code& operator=(code&& code) noexcept;
		bool operator==(const code& code) const = delete;
		bool operator!=(const code& code) const = delete;

	public:
		grapheme at(std::size_t x, std::size_t y) const;

	private:
		void parse_codes_(const std::u32string_view& codes);

	private:
		std::vector<std::u32string> codes_;
	};
}

#endif