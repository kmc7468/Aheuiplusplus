#include <Aheuiplusplus/code.hpp>

#include <utility>

#include <u5e/basic_grapheme_iterator.hpp>

namespace app
{
	code::code(const std::u32string_view& codes)
	{
		parse_codes_(codes);
	}
	code::code(const code& code)
		: codes_(code.codes_)
	{}
	code::code(code&& code) noexcept
		: codes_(std::move(code.codes_))
	{}

	code& code::operator=(const std::u32string_view& codes)
	{
		parse_codes_(codes);
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

	grapheme code::at(std::size_t x, std::size_t y) const
	{
		const std::u32string& line = codes_[y];
		u5e::basic_grapheme_iterator<std::u32string> iterator(line.begin(), line.end());

		for (std::size_t i = 0; i < x; ++i)
		{
			++iterator;
		}

		return *iterator;
	}

	void code::parse_codes_(const std::u32string_view& codes)
	{
		codes_.clear();

		std::size_t pos = 0;
		std::size_t line_pos;
		
		while ((line_pos = codes.find(U'\n', pos)) != std::u32string_view::npos)
		{
			std::u32string& line = codes_.emplace_back(codes.substr(pos, line_pos - pos));
			pos = line_pos + 1;

			if (!line.empty() && line.back() == U'\r')
			{
				line.erase(line.end() - 1);
			}
		}

		if (pos < codes.size())
		{
			codes_.emplace_back(codes.substr(pos));
		}
	}
}