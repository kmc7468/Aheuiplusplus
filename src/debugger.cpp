#include <Aheuiplusplus/debugger.hpp>

#include <utility>

namespace app
{
	debugger::debugger(const code_view& code, const command_line& command_line)
		: interpreter_(this, command_line)
	{
		interpreter_.code(code);
	}
	debugger::debugger(code&& code, const command_line& command_line)
		: interpreter_(this, command_line)
	{
		interpreter_.code(std::move(code));
	}

	const std::vector<point>& debugger::breakpoints() const noexcept
	{
		return breakpoints_;
	}
	std::vector<point>& debugger::breakpoints() noexcept
	{
		return breakpoints_;
	}
}