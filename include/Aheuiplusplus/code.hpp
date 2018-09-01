#ifndef AHEUIPLUSPLUS_HEADER_CODE_HPP
#define AHEUIPLUSPLUS_HEADER_CODE_HPP

#include <Aheuiplusplus/cursor.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <u5e/basic_grapheme.hpp>
#include <u5e/basic_grapheme_iterator.hpp>

namespace app
{
	using u5e::basic_grapheme;
	using grapheme = u5e::basic_grapheme<std::u32string>;

	namespace details
	{
		template<typename Ty_>
		struct is_basic_string
		{
			static constexpr bool value = false;
		};
		template<typename CharType_, typename Traits_, typename Allocator_>
		struct is_basic_string<std::basic_string<CharType_, Traits_, Allocator_>>
		{
			static constexpr bool value = true;
		};

		template<typename Ty_>
		struct is_basic_string_view
		{
			static constexpr bool value = false;
		};
		template<typename CharType_, typename Traits_>
		struct is_basic_string_view<std::basic_string_view<CharType_, Traits_>>
		{
			static constexpr bool value = true;
		};
	}

	template<typename String_>
	class basic_code final
	{
	public:
		using string_type = String_;

	public:
		basic_code() = default;
		basic_code(const String_& codes)
		{
			parse_codes_(codes);
		}
		template<typename Code_, typename = typename std::enable_if<
			(details::is_basic_string<typename Code_::string_type>::value &&
			details::is_basic_string_view<String_>::value) ||
			(details::is_basic_string_view<typename Code_::string_type>::value &&
			details::is_basic_string<String_>::value)
		>::type>
		basic_code(const Code_& code)
		{
			make_from_code_(code);
		}
		basic_code(const basic_code& code)
			: codes_(code.codes_)
		{}
		basic_code(basic_code&& code) noexcept
			: codes_(std::move(code.codes_))
		{}
		~basic_code() = default;

	public:
		basic_code& operator=(const String_& codes)
		{
			parse_codes_(codes);
			return *this;
		}
		basic_code& operator=(const basic_code& code)
		{
			codes_ = code.codes_;
			return *this;
		}
		basic_code& operator=(basic_code&& code) noexcept
		{
			codes_ = std::move(code.codes_);
			return *this;
		}
		bool operator==(const basic_code& code) const = delete;
		bool operator!=(const basic_code& code) const = delete;

	public:
		bool empty() const noexcept
		{
			return codes_.empty();
		}
		void clear() noexcept
		{
			codes_.clear();
			codes_grapheme_.clear();
		}
		grapheme at(std::size_t x, std::size_t y) const
		{
			return codes_grapheme_.at(y).at(x);
		}
		grapheme at(const point& location) const
		{
			return at(location.x(), location.y());
		}
		grapheme at(const cursor& location) const
		{
			return at(location.x(), location.y());
		}
		const std::vector<grapheme>& at(std::size_t y) const
		{
			return codes_grapheme_.at(y);
		}
		const std::vector<grapheme>& line(std::size_t y) const
		{
			return at(y);
		}
		const std::vector<grapheme>& line(const point& location) const
		{
			return at(location.y());
		}
		const std::vector<grapheme>& line(const cursor& location) const
		{
			return at(location.y());
		}

		std::size_t width(std::size_t y) const
		{
			return at(y).size();
		}
		std::size_t height(std::size_t x) const
		{
			for (std::size_t i = codes_grapheme_.size() - 1; i >= 0; --i)
			{
				if (at(i).size() > x)
				{
					return i + 1;
				}
				else if (i == 0)
				{
					return 0;
				}
			}
		}

	private:
		void parse_codes_(const String_& codes)
		{
			clear();

			std::size_t pos = 0;
			std::size_t line_pos;

			using grapheme_iterator = u5e::basic_grapheme_iterator<String_>;

			static constexpr auto cr =
				static_cast<typename String_::value_type>('\r');
			static constexpr auto lf =
				static_cast<typename String_::value_type>('\n');

			while ((line_pos = codes.find(lf, pos)) != String_::npos)
			{
				String_& line = codes_.emplace_back(codes.substr(pos, line_pos - pos));
				pos = line_pos + 1;

				if (!line.empty() && line.back() == cr)
				{
					line.erase(line.end() - 1);
				}

				const grapheme_iterator end = grapheme_iterator(line.begin(), line.end(), line.end());
				std::vector<basic_grapheme<String_>>& line_grapheme = codes_grapheme_.emplace_back();

				for (grapheme_iterator iter(line.begin(), line.end()); iter != end; ++iter)
				{
					line_grapheme.push_back(*iter);
				}
			}

			if (pos < codes.size())
			{
				String_& line = codes_.emplace_back(codes.substr(pos));

				const grapheme_iterator end = grapheme_iterator(line.begin(), line.end(), line.end());
				std::vector<basic_grapheme<String_>>& line_grapheme = codes_grapheme_.emplace_back();

				for (grapheme_iterator iter(line.begin(), line.end()); iter != end; ++iter)
				{
					line_grapheme.push_back(*iter);
				}
			}
		}
		template<typename Code_>
		void make_from_code_(const Code_& code)
		{
			clear();

			using grapheme_iterator = u5e::basic_grapheme_iterator<String_>;
			
			for (const auto& line_org : code.codes())
			{
				String_& line = codes_.emplace_back(line_org);

				const grapheme_iterator end = grapheme_iterator(line.begin(), line.end(), line.end());
				std::vector<basic_grapheme<String_>>& line_grapheme = codes_grapheme_.emplace_back();

				for (grapheme_iterator iter(line.begin(), line.end()); iter != end; ++iter)
				{
					line_grapheme.push_back(*iter);
				}
			}
		}

	public:
		const std::vector<String_>& codes() const noexcept
		{
			return codes_;
		}
		const std::vector<std::vector<basic_grapheme<String_>>>& codes_grapheme() const noexcept
		{
			return codes_grapheme_;
		}

	private:
		std::vector<String_> codes_;
		std::vector<std::vector<basic_grapheme<String_>>> codes_grapheme_;
	};

	using code = basic_code<std::u32string>;
	using code_view = basic_code<std::u32string_view>;
}

#endif