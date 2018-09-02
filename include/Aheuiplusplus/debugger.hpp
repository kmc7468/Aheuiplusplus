#ifndef AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP
#define AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/command_line.hpp>
#include <Aheuiplusplus/cursor.hpp>
#include <Aheuiplusplus/interpreter.hpp>

#include <vector>

namespace app
{
	class debugger final
	{
	public:
		debugger() = default;
		debugger(const code_view& code, const command_line& command_line);
		debugger(code&& code, const command_line& command_line);
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
	};
}

#endif