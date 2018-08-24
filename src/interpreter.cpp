#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

#include <utility>

namespace app
{
	interpreter::interpreter(debugger* debugger)
		: debugger_(debugger)
	{}
	interpreter::interpreter(debugger* debugger, const app::code& code)
		: debugger_(debugger), code_(code)
	{}
	interpreter::interpreter(debugger* debugger, app::code&& code)
		: debugger_(debugger), code_(std::move(code))
	{}

	const app::code& interpreter::code() const noexcept
	{
		return code_;
	}
	void interpreter::code(const app::code& new_code)
	{
		code_ = new_code;
	}
	void interpreter::code(app::code&& new_code) noexcept
	{
		code_ = std::move(new_code);
	}
}