#include <Aheuiplusplus/namespace.hpp>

#include <utility>

namespace app
{
	namespace_info::namespace_info(const code_view& name)
		: name_(name)
	{}
	namespace_info::namespace_info(code&& name)
		: name_(std::move(name))
	{}

	code_view namespace_info::name() const
	{
		return name_;
	}

	const std::vector<std::shared_ptr<namespace_info>>& namespace_info::namespaces() const noexcept
	{
		return namespaces_;
	}
	std::vector<std::shared_ptr<namespace_info>>& namespace_info::namespaces() noexcept
	{
		return namespaces_;
	}
	const std::vector<function_ptr>& namespace_info::functions() const noexcept
	{
		return functions_;
	}
	std::vector<function_ptr>& namespace_info::functions() noexcept
	{
		return functions_;
	}
}