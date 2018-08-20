#include <Aheuiplusplus/code.hpp>

#include <u5e/basic_grapheme_iterator.hpp>

#include <utility>

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
		return codes_grapheme_[y][x];
	}
	grapheme code::at(const point& location) const
	{
		return at(location.x(), location.y());
	}
	grapheme code::at(const cursor& location) const
	{
		return at(location.x(), location.y());
	}

	void code::parse_codes_(const std::u32string_view& codes)
	{
		codes_.clear();

		std::size_t pos = 0;
		std::size_t line_pos;
		
		using grapheme_iterator = u5e::basic_grapheme_iterator<std::u32string>;

		while ((line_pos = codes.find(U'\n', pos)) != std::u32string_view::npos)
		{
			std::u32string& line = codes_.emplace_back(codes.substr(pos, line_pos - pos));
			pos = line_pos + 1;

			if (!line.empty() && line.back() == U'\r')
			{
				line.erase(line.end() - 1);
			}

			const grapheme_iterator end = grapheme_iterator(line.begin(), line.end(), line.end());
			std::vector<grapheme>& line_grapheme = codes_grapheme_.emplace_back();

			for (grapheme_iterator iter(line.begin(), line.end()); iter != end; ++iter)
			{
				line_grapheme.push_back(*iter);
			}
		}

		if (pos < codes.size())
		{
			std::u32string& line = codes_.emplace_back(codes.substr(pos));

			const grapheme_iterator end = grapheme_iterator(line.begin(), line.end(), line.end());
			std::vector<grapheme>& line_grapheme = codes_grapheme_.emplace_back();

			for (grapheme_iterator iter(line.begin(), line.end()); iter != end; ++iter)
			{
				line_grapheme.push_back(*iter);
			}
		}
	}
}