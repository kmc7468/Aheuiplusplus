#include <Aheuiplusplus/debugger.hpp>

#include <utility>

namespace app
{
	debugger::debugger(const code& code)
		: interpreter_(this, code)
	{}
	debugger::debugger(code&& code)
		: interpreter_(this, std::move(code))
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