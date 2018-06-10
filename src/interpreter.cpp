#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	interpreter::interpreter(app::version version)
		: version_(version)
	{}

	app::version interpreter::version() const noexcept
	{
		return version_;
	}
	const std::vector<function*>& interpreter::functions() const noexcept
	{
		return functions_;
	}
}