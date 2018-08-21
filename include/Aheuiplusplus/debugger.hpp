#ifndef AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP
#define AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/cursor.hpp>
#include <Aheuiplusplus/interpreter.hpp>

#include <string_view>
#include <vector>

namespace app
{
	class debugger final
	{
	public:
		debugger(const std::u32string_view& code);
		debugger(std::u32string&& code);
		debugger(const debugger& debugger) = delete;
		debugger(debugger&& debugger) noexcept = delete;
		~debugger() = default;

	public:
		debugger& operator=(const debugger& debugger) = delete;
		debugger& operator=(debugger&& debugger) noexcept = delete;
		bool operator==(const debugger& debugger) const = delete;
		bool operator!=(const debugger& debugger) const = delete;

	public:
		const std::vector<point>& breakpoints() const noexcept;
		std::vector<point>& breakpoints() noexcept;

	private:
		std::vector<point> breakpoints_;
		grapheme previous_command_;
		grapheme current_command_;

		interpreter interpreter_;
		std::u32string code_;
	};
}

#endif