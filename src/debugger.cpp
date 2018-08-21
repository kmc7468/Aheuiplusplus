#include <Aheuiplusplus/debugger.hpp>

#include <utility>

namespace app
{
	debugger::debugger(const std::u32string_view& code)
		: code_(code)
	{}
	debugger::debugger(std::u32string&& code)
		: code_(std::move(code))
	{}

	const std::vector<point>& debugger::breakpoints() const noexcept
	{
		return breakpoints_;
	}
	std::vector<point>& debugger::breakpoints() noexcept
	{
		return breakpoints_;
	}
}