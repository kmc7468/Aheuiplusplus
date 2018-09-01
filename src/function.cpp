#include <Aheuiplusplus/function.hpp>

#include <Aheuiplusplus/storage.hpp>

#include <utility>

namespace app
{
	named_function::named_function(const code_view& name, const code_view& code)
		: name_(name), code_(code)
	{}
	named_function::named_function(code&& name, const code_view& code)
		: name_(std::move(name)), code_(code)
	{}
	named_function::named_function(const code_view& name, code&& code)
		: name_(name), code_(std::move(code))
	{}
	named_function::named_function(code&& name, code&& code) noexcept
		: name_(std::move(name)), code_(std::move(code))
	{}

	code named_function::name() const
	{
		return name_;
	}
	function_type named_function::type() const noexcept
	{
		return function_type::named_function;
	}
	void named_function::call(storages& storages)
	{
		// TODO
	}
}

namespace app
{
	unnamed_function::unnamed_function(const code_view& code)
		: code_(code)
	{}
	unnamed_function::unnamed_function(code&& code) noexcept
		: code_(std::move(code))
	{}

	code unnamed_function::name() const
	{
		return code();
	}
	function_type unnamed_function::type() const noexcept
	{
		return function_type::unnamed_function;
	}
	void unnamed_function::call(storages& storages)
	{
		// TODO
	}
}

namespace app
{
	native_function::native_function(const code_view& name, const native_function_object& function)
		: name_(name), function_(function)
	{}
	native_function::native_function(const code_view& name, native_function_object&& function)
		: name_(name), function_(std::move(function))
	{}
	native_function::native_function(code&& name, const native_function_object& function)
		: name_(std::move(name)), function_(function)
	{}
	native_function::native_function(code&& name, native_function_object&& function)
		: name_(std::move(name)), function_(std::move(function))
	{}

	code native_function::name() const
	{
		return name_;
	}
	function_type native_function::type() const noexcept
	{
		return function_type::native_function;
	}
	void native_function::call(storages& storages)
	{
		function_(storages);
	}
}